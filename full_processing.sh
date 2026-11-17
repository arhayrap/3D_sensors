attempt="ZXAxisScan"

z_pos=${1}
x_pos=${2}

input_lecroy="F*--AC_LGAD_40um_strip_NEv2000_BV115000mV_Laser20pc_Temp16C_Zpos${z_pos}_Xpos${x_pos}00000.trc"

data_folder="./${attempt}"

mkdir -p ${data_folder}

# cp /home/daq/LecroyMount/${input_lecroy} ${data_folder}
cp ./${attempt}/${input_lecroy} ${data_folder}

echo ${input_lecroy}

python3 conversion.py --inputFilePattern ${input_lecroy} --input_folder ${data_folder}

../TimingDAQ/StandaloneDat2Root --input_file=./Converted_data/conv_F_AC_LGAD_40um_strip_NEv2000_BV115000mV_Laser20pc_Temp16C_Zpos${z_pos}_Xpos${x_pos}00000.root \
                     --output_file=./Processed_data/proc_F_AC_LGAD_40um_strip_NEv2000_BV115000mV_Laser20pc_Temp16C_Zpos${z_pos}_Xpos${x_pos}00000.root \
                     --config=./config/LecroyLaserScan.config --num_channels=8 --num_samples=5002 --num_times=1 --verbose=true --save_meas=true
