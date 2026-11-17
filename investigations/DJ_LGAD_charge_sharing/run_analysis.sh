export data_location="/Users/PITLab/3D_sensors/Processed_data/"

export filename="proc_Keysight_C_DJ100W50B9_bv117_mcp4400_trigExtCh2_MCP_channel_only_10mV_HighSensHighBandwidth_zoomedin_10p5mV.root"
export output_folder="./Output_plots"

export n_bins=60
export rebin=2
export rms_range_high=1.0
export rms_range_low=1.5
export trigger=1
export mcp=7
export amplitude_fraction=0.5

# export conditions="baseline_RMS[${trigger}]<3 && amp[${trigger}]<40 && amp[$((trigger-1))]<40 && amp[$((trigger+1))]<40" #  && amp[${trigger}]>10.5 && amp[$((trigger-1))]>10.5 && amp[$((trigger+1))]>10.5"
export conditions="baseline_RMS[${trigger}]<3 && amp[${trigger}]<39 && amp[$((trigger-1))]<39 && amp[$((trigger+1))]<39 && LP2_50[${trigger}]<0" #  && risetime[$((trigger-1))]<-1 && risetime[$((trigger+1))]<-1" #  && amp[$((trigger+1))]>10.5" #  && amp[$((trigger-1))]<10.5" # && amp[${trigger}]>10.5 && amp[$((trigger-1))]>10.5 && amp[$((trigger+1))]>10.5"

echo $conditions

root -l -b -q "AmplitudeNoise.C(\"${data_location}/${filename}\", \"${output_folder}\", \"${conditions}\", ${n_bins}, ${rebin}, ${rms_range_low}, ${rms_range_high}, ${trigger}, ${mcp}, ${amplitude_fraction})"
root -l -b -q "AmplitudeComparison.C(\"${data_location}/${filename}\", \"${output_folder}\", \"${conditions}\", ${n_bins}, ${rebin}, ${rms_range_low}, ${rms_range_high}, ${trigger}, ${mcp}, ${amplitude_fraction})"
root -l -b -q "WaveformComparison.C(\"${data_location}/${filename}\", \"${output_folder}\", \"${conditions}\", ${n_bins}, ${rebin}, ${rms_range_low}, ${rms_range_high}, ${trigger}, ${mcp}, ${amplitude_fraction})"
