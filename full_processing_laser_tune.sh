
z_pos=${1}
x_pos=${2}
y_pos=${3}
lt=${4}
tag=${5}

attempt="ZXAxisScan"
attempt="ZAxisScan"
attempt="WiderZRange"
attempt="/home/daq/LecroyMount/"
attempt="/home/daq/3D_sensors/${tag}"

# attempt="PeakAmpWide"

# input_lecroy="F*--AC_LGAD_40um_strip_NEv1000_BV115000mV_Laser20pc_Temp16C_Zpos${z_pos}_Xpos${x_pos}00000.trc"
input_lecroy="F*--AC_LGAD_40um_strip_${tag}_NEv1000_BV115000mV_Laser${lt}pc_Temp16C_Zpos${z_pos}_Xpos${x_pos}_Ypos${y_pos}00000.trc"

data_folder="${attempt}"

# mkdir -p ${data_folder}
# cp /home/daq/LecroyMount/${input_lecroy} ${data_folder}
# cp ./${attempt}/${input_lecroy} ${data_folder}

echo ${input_lecroy}

python3 /home/daq/3D_sensors/conversion.py --inputFilePattern ${input_lecroy} --input_folder ${data_folder}
# /home/daq/TimingDAQ
# /home/daq/TimingDAQ
/home/daq/3D_sensors/StandaloneDat2Root --input_file=/home/daq/3D_sensors/Converted_data/conv_F_AC_LGAD_40um_strip_${tag}_NEv1000_BV115000mV_Laser${lt}pc_Temp16C_Zpos${z_pos}_Xpos${x_pos}_Ypos${y_pos}00000.root \
                                --output_file=/home/daq/3D_sensors/Processed_data/proc_F_AC_LGAD_40um_strip_${tag}_NEv1000_BV115000mV_Laser${lt}pc_Temp16C_Zpos${z_pos}_Xpos${x_pos}_Ypos${y_pos}00000.root \
                                --config=/home/daq/3D_sensors/config/LecroyLaserScan.config --num_channels=8 --num_samples=4631 --num_times=1 --verbose=true --save_meas=true

# /home/daq/3D_sensors/StandaloneDat2Root --input_file=/home/daq/3D_sensors/Converted_data/conv_F_AC_LGAD_40um_strip_NEv500_BV115000mV_Laser95pc_Temp16C_Zpos${z_pos}_Xpos${x_pos}_Ypos${y_pos}00000.root \
#                                 --output_file=/home/daq/3D_sensors/Processed_data/proc_F_AC_LGAD_40um_strip_NEv500_BV115000mV_Laser95pc_Temp16C_Zpos${z_pos}_Xpos${x_pos}_Ypos${y_pos}00000.root \
#                                 --config=/home/daq/3D_sensors/config/LecroyLaserScan.config --num_channels=8 --num_samples=5002 --num_times=1 --verbose=true --save_meas=true
