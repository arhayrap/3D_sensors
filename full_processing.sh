attempt=3
# input_lecroy="F*--DJ_LGAD_50um_strip_NEv20000_BV117000mV_MCP440000002.trc"
# input_lecroy="F*--DJ_LGAD_50um_strip_NEv20000_BV117000mV_MCP4400.trc"
# input_lecroy="F*--DJ_LGAD_50um_strip_NEv15000_BV117000mV_MCP4400_${attempt}.trc"
input_lecroy="C*--DJ_LGAD_50um_strip_NEv15000_BV117000mV_MCP4400_${attempt}--00001.trc"
# input_keysight="DJ_LGAD_50um_strip_Ch*_NEv20000_BV117000mV_MCP4400.h5"
# input_keysight="DJ_LGAD_50um_strip_Ch*_NEv200_BV117000mV_MCP4400.h5"
# input_keysight="DJ_LGAD_50um_strip_Ch*_NEv20000_BV117000mV_MCP4400_attempt_2.h5"
# input_keysight="C*_DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_10mV.h5"
# input_keysight="C*_DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_10mV.h5"
# input_keysight="C*_DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_10mV_ZoomHighSensHighBandwidth.h5"

input_keysight="C*_DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_12mV_ZoomHighSensHighBandwidth_${attempt}.h5"

# data_folder="./DJ_LGAD_W50_117V_Sr90_3mm_10mV_TrigCh2_HighSens_zoomedin_10mV_20000_7"
# data_folder="./DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_12mV_ZoomHighSensHighBandwidth_${attempt}"

data_folder="./DJ100W50B9_bv117_mcp4400_trigExt_Ev5000_12mV_ZoomHighSensHighBandwidth_${attempt}"

mkdir -p ${data_folder}

cp /Volumes/Verbatim/${input_lecroy} ./${data_folder}
# cp /Volumes/Verbatim/${input_keysight} ./${data_folder}

python3 conversion.py --inputFilePattern ${input_lecroy} --input_folder ${data_folder}

# python3 convertH5ToROOT.py ./${data_folder}/DJ_LGAD_50um_strip_Ch1_NEv20000_BV117000mV_MCP4400.h5 \
# 			   ./${data_folder}/DJ_LGAD_50um_strip_Ch2_NEv20000_BV117000mV_MCP4400.h5 \
# 			   ./${data_folder}/DJ_LGAD_50um_strip_Ch3_NEv20000_BV117000mV_MCP4400.h5 \
# 			   ./${data_folder}/DJ_LGAD_50um_strip_Ch4_NEv20000_BV117000mV_MCP4400.h5 \
# 			   -o ./Converted_data/conv_DJ_LGAD_50um_strip_NEv20000_BV117000mV_MCP4400.root

# python3 convertH5ToROOT.py ./${data_folder}/DJ_LGAD_50um_strip_Ch1_NEv200_BV117000mV_MCP4400.h5 \
# 			   ./${data_folder}/DJ_LGAD_50um_strip_Ch2_NEv200_BV117000mV_MCP4400.h5 \
# 			   ./${data_folder}/DJ_LGAD_50um_strip_Ch3_NEv200_BV117000mV_MCP4400.h5 \
# 			   ./${data_folder}/DJ_LGAD_50um_strip_Ch4_NEv200_BV117000mV_MCP4400.h5 \
# 			   -o ./Converted_data/conv_DJ_LGAD_50um_strip_NEv200_BV117000mV_MCP4400.root


# python3 convertH5ToROOT.py ./${data_folder}/C1_DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_10mV.h5 \
# 			   ./${data_folder}/C2_DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_10mV.h5 \
# 			   ./${data_folder}/C3_DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_10mV.h5 \
# 			   ./${data_folder}/C4_DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_10mV.h5 \
# 			   -o ./Converted_data/conv_DJ_LGAD_50um_strip_NEv20000_BV117000mV_MCP4400.root

# python3 convertH5ToROOT.py ./${data_folder}/C1_DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_12mV_ZoomHighSensHighBandwidth_${attempt}.h5 \
#                            ./${data_folder}/C2_DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_12mV_ZoomHighSensHighBandwidth_${attempt}.h5 \
#                            ./${data_folder}/C3_DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_12mV_ZoomHighSensHighBandwidth_${attempt}.h5 \
#                            ./${data_folder}/C4_DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_12mV_ZoomHighSensHighBandwidth_${attempt}.h5 \
#                            -o ./Converted_data/conv_DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_12mV_ZoomHighSensHighBandwidth_${attempt}.root


# ./StandaloneDat2Root --input_file=./Converted_data/conv_DJ_LGAD_50um_strip_NEv20000_BV117000mV_MCP4400.root  \
#                      --output_file=./Processed_data/proc_DJ_LGAD_50um_strip_NEv20000_BV117000mV_MCP4400.root \ 
#                      --config=./config/KeysightCh2.config --num_channels=4 --num_samples=5120 --num_times=1 --verbose=true --save_meas=true

# ./StandaloneDat2Root --input_file=./Converted_data/conv_DJ_LGAD_50um_strip_NEv200_BV117000mV_MCP4400.root --output_file=./Processed_data/proc_DJ_LGAD_50um_strip_NEv200_BV117000mV_MCP4400.root --config=./config/KeysightCh2.config --num_channels=4 --num_samples=5120 --num_times=1 --verbose=true --save_meas=true

# ./StandaloneDat2Root --input_file=./Converted_data/conv_DJ_LGAD_50um_strip_NEv20000_BV117000mV_MCP4400.root --output_file=./Processed_data/proc_DJ_LGAD_50um_strip_NEv20000_BV117000mV_MCP4400.root --config=./config/KeysightCh2.config --num_channels=4 --num_samples=5120 --num_times=1 --verbose=true --save_meas=true

# rm ./Processed_data/proc_DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_12mV_ZoomHighSensHighBandwidth_${attempt}.root
# ./StandaloneDat2Root --input_file=./Converted_data/conv_DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_10mV_ZoomHighSensHighBandwidth_${attempt}.root --output_file=./Processed_data/proc_DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_10mV_ZoomHighSensHighBandwidth_${attempt}.root --config=./config/KeysightCh2.config --num_channels=4 --num_samples=5120 --num_times=1 --verbose=true --save_meas=true
# ./StandaloneDat2Root --input_file=./Converted_data/conv_DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_12mV_ZoomHighSensHighBandwidth_${attempt}.root --output_file=./Processed_data/proc_DJ100W50B9_bv117_mcp4400_trigExt_Ev20000_12mV_ZoomHighSensHighBandwidth_${attempt}.root --config=./config/KeysightCh2_10ns.config --num_channels=4 --num_samples=1000 --num_times=1 --verbose=true --save_meas=true

./StandaloneDat2Root --input_file=./Converted_data/conv_F_DJ_LGAD_50um_strip_NEv5000_BV117000mV_MCP4400.root \
                     --output_file=./Processed_data/proc_F_DJ_LGAD_50um_strip_NEv5000_BV117000mV_MCP4400.root \
                     --config=./config/LecroyMCP.config   --num_channels=8 --num_samples=5002 --num_times=1 --verbose=true --save_meas=true


