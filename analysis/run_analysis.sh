export data_location="/Users/PITLab/3D_sensors/Processed_data/"
# export data_location="/Users/PITlab/3D_sensors/first_files/"

# export filename="proc_F_AC100W40_strip3mm_bv110_mcp4400_trigCh4_Ch8_Ru106_4days.root"
# export output_folder="./AC100W40_110V_0p3mm_Ru106"

# export filename="proc_F_AC100W40_strip3mm_bv120_mcp4400_trigCh4_Ch8_Ru106_3days.root"
# export output_folder="./AC100W40_120V_0p3mm_Ru106"

# export filename="proc_F_AC100W40_strip3mm_bv108_mcp4400_trigCh4_Ch8_Ru106_low_threshold.root"
# export output_folder="./AC100W40_108V_0p3mm_Ru106"

# export filename="proc_F_AC100W40_strip3mm_bv105_mcp4400_trigCh4_Ch8_Ru106_low_threshold.root"
# export output_folder="./AC100W40_105V_0p3mm_Ru106"

# export filename="proc_F_AC100W40_strip3mm_bv103_mcp4400_trigCh4_Ch8_Ru106_low_threshold.root"
# export output_folder="./AC100W40_103V_0p3mm_Ru106"

# export filename="proc_F_AC100W40_strip3mm_bv110_mcp4400_trigCh4_Ch8_Ru106_low_threshold_retaken.root"
# export output_folder="./AC100W40_110V_0p3mm_Ru106_retaken"

# export filename="proc_F_AC100W80_strip3mm_bv105_mcp4400_trigCh4_Ch8_Ru106_low_threshold.root"
# export output_folder="./AC100W80_105V_0p3mm_Ru106"

# export filename="prep_AC100W80_strip3mm_bv116_mcp4400_linear.root"
# export output_folder="AC100W80_116V_0p3mm_Ce137"

# export filename="proc_F_AC100W80_strip3mm_bv113_mcp4400_trigCh4_Ch8_Ru106_low_threshold.root"
# export output_folder="./AC100W80_113V_0p3mm_Ru106"

export filename="proc_F_AC100W80_strip3mm_bv116_mcp4400_trigCh4_Ch8_Ru106_low_threshold.root"
export output_folder="./AC100W80_116V_0p3mm_Ru106"

export n_bins=15
export rebin=5
export rms_range_high=1.0
export rms_range_low=1.5
export trigger=3
export mcp=7
export amplitude_fraction=0.5

root -l -b -q "AmpFraction.C(\"${data_location}/${filename}\", \"${output_folder}\", ${trigger}, ${mcp})"

root -l -b -q "DataOverview.C(\"${data_location}/${filename}\", \"${output_folder}\", ${n_bins}, ${rebin}, ${rms_range_low}, ${rms_range_high}, ${trigger}, ${mcp}, ${amplitude_fraction})"

rm ./CombinedDatasets/*.png; root -l -b -q "DataComparison.C"