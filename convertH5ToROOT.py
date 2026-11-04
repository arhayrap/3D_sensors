#!/usr/bin/env python3

import argparse
import os
import re
from pathlib import Path

import ROOT
import h5py
import numpy as np

'''
Converts h5 files from oscilloscope to root files with the structure for TimingDAQ further analysis.

Arguments accepted:
●   h5_files (as many files as needed, but all the h5 files should satisfy the Conditions written below),
●   -o /path/to/output/root/file

Conditions:
●   The input h5 files should have different ["Waveforms"]["Channel X"] datasets. For example:
    if two input h5 files have ["Waveforms"]["Channel 1"] dataset, the conversion will crash.
●   The input h5 files should have the same:
    ○   number of segments,
    ○   samples per segment,
    ○   time-axis settings for every channel and segment.

Example usage (need to activate the conda environment first):

●   "python ./src/convertH5ToROOT.py ../Test4Ch/Ch1.h5 ../Test4Ch/Ch4.h5 ../Test4Ch/Ch2.h5 -o ../Converted/conv.root"

This will fill like channel[0] with "/Test4Ch/Ch1.h5" data, channel[1] with "/Test4Ch/Ch4.h5" data,
channel[2] with "/Test4Ch/Ch2.h5" data and will not fill a channel[3].
Meaning will fill the channels in order of the input files.


'''


# ----------------------------------------------------------------
# Helper functions
# ----------------------------------------------------------------

def get_channel_number(channel_group_name):
    """Extracts the channel number from a name such as 'Channel 2'."""

    match = re.fullmatch(r"Channel\s+(\d+)", channel_group_name)

    if match is None:
        return None

    return int(match.group(1))


def get_segment_number(dataset_name):
    """Extracts the segment number from a dataset name."""

    match = re.search(r"Seg(\d+)Data$", dataset_name)

    if match is None:
        raise ValueError(
            f"Could not determine segment number from:\n"
            f"{dataset_name}"
        )

    return int(match.group(1))


def remove_channel_from_filename(filename):
    """
    Removes channel identifiers such as '-Ch2' or '_Channel3'
    from a filename.
    """

    stem = Path(filename).stem

    stem = re.sub(
        r"(?i)(^|[-_])ch(?:annel)?\s*\d+(?=[-_]|$)",
        r"\1",
        stem
    )

    stem = re.sub(
        r"[-_]{2,}",
        "-",
        stem
    )

    return stem.strip("-_")


def determine_run_tag(input_filepaths):
    """Constructs the run tag from the input filenames."""

    possible_tags = [
        remove_channel_from_filename(filepath.name)
        for filepath in input_filepaths
    ]

    if len(set(possible_tags)) == 1:
        return possible_tags[0]

    print("\nWarning: input filenames do not give the same run tag:")

    for filepath, tag in zip(input_filepaths, possible_tags):
        print(f"  {filepath.name} -> {tag}")

    print(
        f"Using the first run tag:\n"
        f"  {possible_tags[0]}"
    )

    return possible_tags[0]


def determine_output_directory(input_filepaths):
    """
    If the input path contains:

        0_RawDataFromOscilloscope

    it is replaced with:

        1_ConvertedRoots

    Otherwise, the ROOT file is written beside the input files.
    """

    input_parents = {
        filepath.parent
        for filepath in input_filepaths
    }

    if len(input_parents) == 1:
        input_directory = next(iter(input_parents))
    else:
        input_directory = Path.cwd()

    path_parts = list(input_directory.parts)

    if "0_RawDataFromOscilloscope" in path_parts:

        raw_directory_index = path_parts.index(
            "0_RawDataFromOscilloscope"
        )

        path_parts[raw_directory_index] = (
            "1_ConvertedRoots"
        )

        return Path(*path_parts)

    return input_directory


def get_h5_attribute(
    h5_objects,
    attribute_names,
    *,
    default=None,
    required=False,
    description="attribute"
):
    """
    Reads the first matching attribute from the supplied HDF5 objects.

    Objects should be ordered from most specific to least specific. For
    example, pass the segment dataset first and the channel group second.
    """

    for h5_object in h5_objects:

        for attribute_name in attribute_names:

            if attribute_name in h5_object.attrs:
                return h5_object.attrs[attribute_name]

    if required:
        searched_names = ", ".join(attribute_names)

        raise KeyError(
            f"Could not find {description}. "
            f"Searched for: {searched_names}"
        )

    return default


def read_time_axis_parameters(
    channel_group,
    segment_dataset,
    physical_channel
):
    """Reads horizontal waveform metadata from one channel and segment."""

    # Channel-group attributes describe the externally usable waveform
    # scaling. Segment-dataset attributes are used only as a fallback for
    # files that store segmented metadata there.
    x_increment = float(
        get_h5_attribute(
            (channel_group, segment_dataset),
            ("XInc",),
            required=True,
            description=(
                f"XInc for physical Channel {physical_channel}"
            )
        )
    )

    x_origin = get_h5_attribute(
        (channel_group,),
        ("XOrg", "XOffset"),
        default=None
    )

    if x_origin is None:
        x_origin = get_h5_attribute(
            (segment_dataset,),
            ("SegmentedXOrg", "XOrg", "XOffset"),
            required=True,
            description=(
                f"X origin/offset for physical Channel "
                f"{physical_channel}"
            )
        )

    x_origin = float(x_origin)

    x_reference = float(
        get_h5_attribute(
            (channel_group, segment_dataset),
            ("XRef", "XReference"),
            default=0.0
        )
    )

    return x_increment, x_origin, x_reference


def construct_time_axis_ns(
    x_increment,
    x_origin,
    x_reference,
    number_of_points
):
    """Constructs the shared time axis in nanoseconds."""

    sample_indices = np.arange(
        number_of_points,
        dtype=np.float64
    )

    time_axis_seconds = (
        x_origin
        + (sample_indices - x_reference) * x_increment
    )

    return time_axis_seconds * 1e9


def horizontal_values_match(value, reference_value):
    """Compares horizontal metadata while allowing tiny float round-off."""

    return bool(
        np.isclose(
            value,
            reference_value,
            rtol=1e-9,
            atol=1e-18
        )
    )


def read_channel_waveform(
    channel_group,
    segment_dataset,
    physical_channel
):
    """
    Reads one waveform using that channel's own vertical ADC parameters.

    Keysight HDF5 files commonly store floating-point values that are already
    expressed in physical Y units. Those values are copied directly.

    If a dataset contains integer ADC codes, its own YInc, YOrg, and optional
    YRef values are used to convert the codes to physical values:

        y = (code - YRef) * YInc + YOrg
    """

    raw_waveform = np.asarray(segment_dataset)

    if not np.issubdtype(raw_waveform.dtype, np.integer):
        return raw_waveform.astype(np.float32, copy=False)

    h5_objects = (
        segment_dataset,
        channel_group
    )

    y_increment = float(
        get_h5_attribute(
            h5_objects,
            ("YInc",),
            required=True,
            description=(
                f"YInc for integer waveform data in physical "
                f"Channel {physical_channel}"
            )
        )
    )

    y_origin = float(
        get_h5_attribute(
            h5_objects,
            ("YOrg", "YOffset"),
            default=0.0
        )
    )

    y_reference = float(
        get_h5_attribute(
            h5_objects,
            ("YRef", "YReference"),
            default=0.0
        )
    )

    waveform = (
        (
            raw_waveform.astype(np.float64)
            - y_reference
        )
        * y_increment
        + y_origin
    )

    return waveform.astype(np.float32)


# ----------------------------------------------------------------
# Main conversion
# ----------------------------------------------------------------

def main():

    parser = argparse.ArgumentParser(
        description=(
            "Convert Keysight segmented HDF5 waveform files "
            "to a ROOT file for TimingDAQ."
        )
    )

    parser.add_argument(
        "h5_files",
        nargs="+",
        help=("Input HDF5 files. The physical channel numbers are detected from the HDF5 contents.")
    )

    parser.add_argument(
        "-o",
        "--output",
        default=None,
        help=("Optional output ROOT filepath or output directory. Normally this is determined automatically.")
    )

    parser.add_argument(
        "--run-number",
        type=int,
        default=0,
        help="ROOT run number. Default: 0"
    )

    arguments = parser.parse_args()

    input_filepaths = [
        Path(filename).expanduser().resolve()
        for filename in arguments.h5_files
    ]

    for filepath in input_filepaths:

        if not filepath.is_file():
            raise FileNotFoundError(
                f"Input file does not exist:\n{filepath}"
            )

    run_tag = determine_run_tag(input_filepaths)

    if arguments.output is None:

        output_directory = determine_output_directory(input_filepaths)

        output_filepath = (
            output_directory
            / f"conv_{run_tag}.root"
        )

    else:

        output_argument = (
            Path(arguments.output)
            .expanduser()
            .resolve()
        )

        if output_argument.suffix.lower() == ".root":
            output_filepath = output_argument
        else:
            output_filepath = (
                output_argument
                / f"conv_{run_tag}.root"
            )

    output_filepath.parent.mkdir(parents=True, exist_ok=True)

    print("\nInput HDF5 files:")

    for filepath in input_filepaths:
        print(f"  {filepath} ----> channel[{input_filepaths.index(filepath)}]")

    print("\nRun tag:")
    print(f"  {run_tag}")

    print("\nOutput ROOT file:")
    print(f"  {output_filepath}")

    # ----------------------------------------------------------------
    # Open input files and find channels
    # ----------------------------------------------------------------

    opened_h5_files = []

    channel_groups = {}
    channel_filepaths = {}
    segment_names = {}
    segment_numbers = {}

    try:

        for filepath in input_filepaths:

            h5_file = h5py.File(filepath, "r")
            opened_h5_files.append(h5_file)

            if "Waveforms" not in h5_file:
                raise KeyError(
                    f"'Waveforms' group not found in:\n"
                    f"{filepath}"
                )

            waveform_group_names = list(h5_file["Waveforms"].keys())

            detected_channels = []

            for group_name in waveform_group_names:

                channel_number = get_channel_number(group_name)

                if channel_number is None:
                    continue

                if channel_number in channel_groups:
                    raise RuntimeError(
                        f"Channel {channel_number} appears in "
                        f"more than one input file."
                    )

                channel_group = (h5_file["Waveforms"][group_name])

                channel_groups[channel_number] = (channel_group)

                channel_filepaths[channel_number] = (filepath)

                detected_channels.append(channel_number)

            if not detected_channels:
                raise RuntimeError(
                    f"No channel groups were found in:\n"
                    f"{filepath}\n\n"
                    f"Waveforms objects:\n"
                    f"{waveform_group_names}"
                )

            print(f"\nFile: {filepath.name}")
            print("Detected physical channels:", detected_channels)

        active_channels = sorted(channel_groups)

        if not active_channels:
            raise RuntimeError("No waveform channels were detected.")

        print("\nStored channel order:")

        for stored_index, physical_channel in enumerate(active_channels):
            print(
                f"  channel[{stored_index}] "
                f"= physical Channel {physical_channel}"
            )

        # ----------------------------------------------------------------
        # Find and sort segment datasets
        # ----------------------------------------------------------------

        for physical_channel in active_channels:

            channel_group = channel_groups[physical_channel]

            numbered_datasets = []

            for dataset_name in channel_group.keys():

                try:
                    segment_number = get_segment_number(dataset_name
)
                except ValueError:
                    continue

                numbered_datasets.append((segment_number, dataset_name))

            numbered_datasets.sort(
                key=lambda item: item[0]
            )

            if not numbered_datasets:
                raise RuntimeError(
                    f"No segment datasets were found for "
                    f"Channel {physical_channel}."
                )

            segment_numbers[physical_channel] = [
                item[0]
                for item in numbered_datasets
            ]

            segment_names[physical_channel] = [
                item[1]
                for item in numbered_datasets
            ]

            print(f"\nChannel {physical_channel}:")
            print("  Source file:", channel_filepaths[physical_channel].name)
            print("  Number of segments:", len(segment_names[physical_channel]))
            print("  First dataset:", segment_names[physical_channel][0])
            print("  Last dataset:", segment_names[physical_channel][-1])

        # ----------------------------------------------------------------
        # Verify that channels contain matching segments
        # ----------------------------------------------------------------

        reference_channel = active_channels[0]

        reference_segment_numbers = segment_numbers[reference_channel]

        for physical_channel in active_channels[1:]:

            if (
                segment_numbers[physical_channel]
                != reference_segment_numbers
            ):
                raise RuntimeError(
                    f"Channel {physical_channel} does not "
                    f"contain the same segment numbers as "
                    f"Channel {reference_channel}."
                )

        number_of_events = len(reference_segment_numbers)

        # ----------------------------------------------------------------
        # Determine waveform length
        # ----------------------------------------------------------------

        first_dataset_name = segment_names[reference_channel][0]

        first_dataset = channel_groups[reference_channel][first_dataset_name]

        if first_dataset.ndim != 1:
            raise RuntimeError(
                f"Expected a one-dimensional waveform, "
                f"but {first_dataset_name} has shape "
                f"{first_dataset.shape}."
            )

        number_of_points = first_dataset.shape[0]

        for physical_channel in active_channels:

            channel_first_dataset = (
                channel_groups[physical_channel][
                    segment_names[physical_channel][0]
                ]
            )

            if (
                channel_first_dataset.shape[0]
                != number_of_points
            ):
                raise RuntimeError(
                    f"Channel {physical_channel} has "
                    f"{channel_first_dataset.shape[0]} points, "
                    f"while Channel {reference_channel} has "
                    f"{number_of_points} points."
                )

        print("\nWaveform information:")
        print("  Number of events:", number_of_events)
        print("  Number of points:", number_of_points)
        print("  Data type:", first_dataset.dtype)

        # ----------------------------------------------------------------
        # Read and verify the shared time-axis metadata
        # ----------------------------------------------------------------

        reference_first_dataset = channel_groups[reference_channel][
            segment_names[reference_channel][0]
        ]

        (
            reference_x_increment,
            reference_x_origin,
            reference_x_reference
        ) = read_time_axis_parameters(
            channel_groups[reference_channel],
            reference_first_dataset,
            reference_channel
        )

        shared_time_axis_ns = construct_time_axis_ns(
            reference_x_increment,
            reference_x_origin,
            reference_x_reference,
            number_of_points
        )

        print("\nVerifying one shared time axis:")
        print(
            f"  Reference: physical Channel {reference_channel}, "
            f"{segment_names[reference_channel][0]}"
        )
        print("  XInc:", reference_x_increment * 1e12, "ps")
        print(
            "  XOrg/XOffset:",
            reference_x_origin * 1e9,
            "ns"
        )
        print("  XRef:", reference_x_reference)
        print(
            "  Time axis starts:",
            shared_time_axis_ns[0],
            "ns"
        )
        print(
            "  Time axis ends:",
            shared_time_axis_ns[-1],
            "ns"
        )

        for physical_channel in active_channels:

            channel_group = channel_groups[physical_channel]

            for dataset_name in segment_names[physical_channel]:

                segment_dataset = channel_group[dataset_name]

                (
                    channel_x_increment,
                    channel_x_origin,
                    channel_x_reference
                ) = read_time_axis_parameters(
                    channel_group,
                    segment_dataset,
                    physical_channel
                )

                comparisons = (
                    (
                        "XInc",
                        channel_x_increment,
                        reference_x_increment
                    ),
                    (
                        "XOrg/XOffset/SegmentedXOrg",
                        channel_x_origin,
                        reference_x_origin
                    ),
                    (
                        "XRef",
                        channel_x_reference,
                        reference_x_reference
                    )
                )

                for (
                    parameter_name,
                    channel_value,
                    reference_value
                ) in comparisons:

                    if not horizontal_values_match(
                        channel_value,
                        reference_value
                    ):
                        raise RuntimeError(
                            f"The input files do not share one "
                            f"time axis.\n"
                            f"Physical Channel {physical_channel}, "
                            f"dataset {dataset_name}:\n"
                            f"  {parameter_name} = {channel_value}\n"
                            f"Reference Channel {reference_channel}, "
                            f"dataset "
                            f"{segment_names[reference_channel][0]}:\n"
                            f"  {parameter_name} = {reference_value}"
                        )

            channel_first_dataset = channel_group[
                segment_names[physical_channel][0]
            ]

            channel_y_increment = get_h5_attribute(
                (
                    channel_first_dataset,
                    channel_group
                ),
                ("YInc",),
                default=None
            )

            channel_y_origin = get_h5_attribute(
                (
                    channel_first_dataset,
                    channel_group
                ),
                ("YOrg", "YOffset"),
                default=None
            )

            channel_y_reference = get_h5_attribute(
                (
                    channel_first_dataset,
                    channel_group
                ),
                ("YRef", "YReference"),
                default=None
            )

            print(
                f"  Physical Channel {physical_channel}: "
                f"horizontal metadata matches"
            )
            print("    YInc:", channel_y_increment)
            print("    YOrg/YOffset:", channel_y_origin)
            print("    YRef:", channel_y_reference)

        print(
            "  All channels and segments use the same time axis."
        )

        # ----------------------------------------------------------------
        # Prepare ROOT file
        # ----------------------------------------------------------------

        output_root_file = ROOT.TFile(
            str(output_filepath),
            "RECREATE"
        )

        if (
            not output_root_file
            or output_root_file.IsZombie()
        ):
            raise RuntimeError(
                f"Could not create ROOT file:\n"
                f"{output_filepath}"
            )

        output_tree = ROOT.TTree("pulse", "pulse")

        number_of_channels = len(active_channels)

        # TimingDAQ uses one common time axis for all channels.
        number_of_time_axes = 1

        i_evt = np.zeros(1, dtype=np.uint32)

        runNum = np.zeros(1, dtype=np.uint32)

        channel = np.zeros(
            (number_of_channels, number_of_points),
            dtype=np.float32
        )

        time_array = np.zeros(
            (
                number_of_time_axes,
                number_of_points
            ),
            dtype=np.float32
        )

        timeoffsets = np.zeros(
            number_of_channels,
            dtype=np.float32
        )

        time_array[0, :] = shared_time_axis_ns.astype(np.float32)
        timeoffsets[:] = 0.0

        # ----------------------------------------------------------------
        # Create ROOT branches
        # ----------------------------------------------------------------

        output_tree.Branch(
            "i_evt",
            i_evt,
            "i_evt/i"
        )

        output_tree.Branch(
            "runNum",
            runNum,
            "runNum/i"
        )

        output_tree.Branch(
            "channel",
            channel,
            (
                f"channel[{number_of_channels}]"
                f"[{number_of_points}]/F"
            )
        )

        output_tree.Branch(
            "time",
            time_array,
            (
                f"time[{number_of_time_axes}]"
                f"[{number_of_points}]/F"
            )
        )

        output_tree.Branch(
            "timeoffsets",
            timeoffsets,
            f"timeoffsets[{number_of_channels}]/F"
        )

        # ----------------------------------------------------------------
        # Fill ROOT tree
        # ----------------------------------------------------------------

        print("\nFilling ROOT tree...")

        for event_index in range(number_of_events):

            i_evt[0] = event_index
            runNum[0] = arguments.run_number

            for (
                stored_channel_index,
                physical_channel
            ) in enumerate(active_channels):

                dataset_name = segment_names[
                    physical_channel
                ][event_index]

                channel_group = channel_groups[physical_channel]
                segment_dataset = channel_group[dataset_name]

                waveform = read_channel_waveform(
                    channel_group,
                    segment_dataset,
                    physical_channel
                )

                if waveform.shape[0] != number_of_points:
                    raise RuntimeError(
                        f"{dataset_name} in Channel "
                        f"{physical_channel} contains "
                        f"{waveform.shape[0]} points instead "
                        f"of {number_of_points}."
                    )

                channel[
                    stored_channel_index,
                    :
                ] = waveform


            output_tree.Fill()

            if (
                event_index == 0
                or (event_index + 1) % 500 == 0
                or event_index + 1 == number_of_events
            ):
                print(
                    f"  Filled {event_index + 1} "
                    f"of {number_of_events} events"
                )

        # ----------------------------------------------------------------
        # Write and close ROOT file
        # ----------------------------------------------------------------

        output_root_file.cd()
        output_tree.Write()
        output_root_file.Close()

        print("\nConversion complete.")
        print("Tree entries:", number_of_events)
        print("Output file:")
        print(output_filepath)

    finally:

        for h5_file in opened_h5_files:

            if h5_file:
                h5_file.close()


if __name__ == "__main__":
    main()
