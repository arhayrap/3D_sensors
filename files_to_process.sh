for i in "C_80tune_MCP_test_attenuator--00000.root" \
         "C_80tune_MCP_test_attenuator_limiter--00000.root" \
         "C_80tune_MCP_test_direct--00000.root" \
         "C_85tune_MCP_test_attenuator--00000.root" \
         "C_85tune_MCP_test_attenuator_limiter--00000.root" \
         "C_85tune_MCP_test_direct--00000.root" \
         "C_90tune_MCP_test_attenuator--00000.root" \
         "C_90tune_MCP_test_attenuator_limiter--00000.root" \
         "C_90tune_MCP_test_direct--00000.root";
do
    echo ${i}
    echo "./StandaloneDat2Root --input_file=./Converted_data/conv_${i} --output_file=./Processed_data/proc_${i} --config=./config/LecroyLaserTest.config --num_channels=4 --num_samples=1002 --num_times=1 --verbose=true --save_meas=true"
    ./StandaloneDat2Root --input_file=./Converted_data/conv_${i} --output_file=./Processed_data/proc_${i} --config=./config/LecroyLaserTest.config --num_channels=4 --num_samples=1002 --num_times=1 --verbose=true --save_meas=true

done;


