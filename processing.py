import os
import sys
import numpy as np
import glob

if __name__ == "__main__":
    path = "/home/daq/LecroyMount/"
    # path = "./ZAxisScan/"
    # path = "./WiderZRange/"
    # path = "./PeakAmpWide/"
    file_pattern = "F*--AC_LGAD_40um_strip_NEv500_BV115000mV_Laser20pc_Temp16C_Zpos*_Xpos*00000.trc"

    x_position = []
    z_position = []

    files = glob.glob(f"{path}{file_pattern}")
    counter = 0
    for file in files:
        z_pos = file.split("_Zpos")[1].split("_")[0]
        x_pos = file.split("_Xpos")[1].split("00000.trc")[0]
        # print(x_pos, z_pos)
        file_to_convert = f"F1--AC_LGAD_40um_strip_NEv500_BV115000mV_Laser20pc_Temp16C_Zpos{z_pos}_Xpos{x_pos}00000.trc"
        file_to_copy = f"proc_F_AC_LGAD_40um_strip_NEv500_BV115000mV_Laser20pc_Temp16C_Zpos{z_pos}_Xpos{x_pos}00000.root"
        # print(file_to_copy)
        print(os.path.exists(f"{path}{file_to_convert}"), f"{path}{file_to_convert}")
        if not os.path.exists(f"{path}{file_to_convert}"): continue
        if not os.path.exists(f"./Processed_data/{file_to_copy}"):
            counter += 1
            print(f"./Processed_data/{file_to_copy}")
            print(counter, x_pos, z_pos)
            os.system(f"bash full_processing.sh {z_pos} {x_pos}")
        
        
        

