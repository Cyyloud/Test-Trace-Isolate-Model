#!/bin/bash

make

if [ $? -ne 0 ]; then
    echo "Error"
    exit 1
fi

# Define "L0" "L1" "L2" "L3" "L4" "L5" "L6" "L7" "L5-SD" "L5-SD-70" "L5-SD-30" "L5-SD-7" "L5-SD-8" "L5-SD-9" "L5-YXL" "L5-YXH" "L5-ZZL" "L5-ZZH" "L5-TDL" "L5-TDH" "L5-Delay42" "L5-Delay12" "L5-Delay24" "L5-Delay21" "L5-Tem" "L0-Tem" "L5-P100" "L5-P0" "L0-P100" "L0-P0"
groups=("L5-SD-7" "L5-SD-8" "L5-SD-9") 

args=(
    # Parameters
    # dirName, p_scale, symbol_NPI, m_infectious_period, sd, enhanced, temporl, fixed, p_test, Pid, mu_ST, mu_TR, time_to_trace

    # Processes
    "data/result/L0/0/ 0.15 0 1.4 0 0 1 0 0.8 0.8 2 2 5"  # 1.30
    "data/result/L0/1/ 0.163 0 1.4 0 0 1 0 0.8 0.8 2 2 5"  # 1.49
    "data/result/L0/2/ 0.195 0 1.4 0 0 1 0 0.8 0.8 2 2 5"  # 1.72
    "data/result/L0/3/ 0.23 0 1.4 0 0 1 0 0.8 0.8 2 2 5"  # 1.91
    "data/result/L0/4/ 0.255 0 1.4 0 0 1 0 0.8 0.8 2 2 5"  # 2.09
    "data/result/L0/5/ 0.285 0 1.4 0 0 1 0 0.8 0.8 2 2 5"  # 2.30
    "data/result/L0/6/ 0.31 0 1.4 0 0 1 0 0.8 0.8 2 2 5"  # 2.49
    "data/result/L0/7/ 0.34 0 1.4 0 0 1 0 0.8 0.8 2 2 5"  # 2.7
    "data/result/L0/8/ 0.37 0 1.4 0 0 1 0 0.8 0.8 2 2 5"  # 2.91
    "data/result/L0/9/ 0.40 0 1.4 0 0 1 0 0.8 0.8 2 2 5"  # 3.12

    "data/result/L1/0/ 0.15 1 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L1/1/ 0.163 1 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L1/2/ 0.195 1 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L1/3/ 0.23 1 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L1/4/ 0.255 1 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L1/5/ 0.285 1 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L1/6/ 0.31 1 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L1/7/ 0.34 1 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L1/8/ 0.37 1 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L1/9/ 0.40 1 1.4 0 0 1 0 0.8 0.8 2 2 5"

    "data/result/L2/0/ 0.15 2 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L2/1/ 0.163 2 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L2/2/ 0.195 2 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L2/3/ 0.23 2 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L2/4/ 0.255 2 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L2/5/ 0.285 2 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L2/6/ 0.31 2 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L2/7/ 0.34 2 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L2/8/ 0.37 2 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L2/9/ 0.40 2 1.4 0 0 1 0 0.8 0.8 2 2 5"

    "data/result/L3/0/ 0.15 3 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L3/1/ 0.163 3 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L3/2/ 0.195 3 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L3/3/ 0.23 3 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L3/4/ 0.255 3 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L3/5/ 0.285 3 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L3/6/ 0.31 3 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L3/7/ 0.34 3 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L3/8/ 0.37 3 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L3/9/ 0.40 3 1.4 0 0 1 0 0.8 0.8 2 2 5"

    "data/result/L4/0/ 0.15 4 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L4/1/ 0.163 4 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L4/2/ 0.195 4 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L4/3/ 0.23 4 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L4/4/ 0.255 4 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L4/5/ 0.285 4 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L4/6/ 0.31 4 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L4/7/ 0.34 4 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L4/8/ 0.37 4 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L4/9/ 0.40 4 1.4 0 0 1 0 0.8 0.8 2 2 5"

    "data/result/L5/0/ 0.15 5 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5/1/ 0.163 5 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5/2/ 0.195 5 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5/3/ 0.23 5 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5/4/ 0.255 5 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5/5/ 0.285 5 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5/6/ 0.31 5 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5/7/ 0.34 5 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5/8/ 0.37 5 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5/9/ 0.40 5 1.4 0 0 1 0 0.8 0.8 2 2 5"

    "data/result/L6/0/ 0.15 6 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L6/1/ 0.163 6 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L6/2/ 0.195 6 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L6/3/ 0.23 6 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L6/4/ 0.255 6 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L6/5/ 0.285 6 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L6/6/ 0.31 6 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L6/7/ 0.34 6 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L6/8/ 0.37 6 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L6/9/ 0.40 6 1.4 0 0 1 0 0.8 0.8 2 2 5"

    "data/result/L7/0/ 0.15 7 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L7/1/ 0.163 7 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L7/2/ 0.195 7 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L7/3/ 0.23 7 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L7/4/ 0.255 7 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L7/5/ 0.285 7 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L7/6/ 0.31 7 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L7/7/ 0.34 7 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L7/8/ 0.37 7 1.4 0 0 1 0 0.8 0.8 2 2 5"
    "data/result/L7/9/ 0.40 7 1.4 0 0 1 0 0.8 0.8 2 2 5"

    # Social Distancing
    
    "data/result/L5-SD/0/ 0.15 5 1.4 2 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD/1/ 0.163 5 1.4 2 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD/2/ 0.195 5 1.4 2 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD/3/ 0.23 5 1.4 2 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD/4/ 0.255 5 1.4 2 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD/5/ 0.285 5 1.4 2 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD/6/ 0.31 5 1.4 2 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD/7/ 0.34 5 1.4 2 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD/8/ 0.37 5 1.4 2 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD/9/ 0.40 5 1.4 2 0 1 0 0.8 0.8 2 2 5"

    "data/result/L5-SD-70/0/ 0.15 5 1.4 3 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-70/1/ 0.163 5 1.4 3 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-70/2/ 0.195 5 1.4 3 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-70/3/ 0.23 5 1.4 3 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-70/4/ 0.255 5 1.4 3 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-70/5/ 0.285 5 1.4 3 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-70/6/ 0.31 5 1.4 3 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-70/7/ 0.34 5 1.4 3 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-70/8/ 0.37 5 1.4 3 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-70/9/ 0.40 5 1.4 3 0 1 0 0.8 0.8 2 2 5"

    "data/result/L5-SD-30/0/ 0.15 5 1.4 1 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-30/1/ 0.163 5 1.4 1 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-30/2/ 0.195 5 1.4 1 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-30/3/ 0.23 5 1.4 1 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-30/4/ 0.255 5 1.4 1 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-30/5/ 0.285 5 1.4 1 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-30/6/ 0.31 5 1.4 1 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-30/7/ 0.34 5 1.4 1 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-30/8/ 0.37 5 1.4 1 0 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-30/9/ 0.40 5 1.4 1 0 1 0 0.8 0.8 2 2 5"

    "data/result/L5-SD-7/0/ 0.15 5 1.4 2 1 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-7/1/ 0.163 5 1.4 2 1 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-7/2/ 0.195 5 1.4 2 1 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-7/3/ 0.23 5 1.4 2 1 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-7/4/ 0.255 5 1.4 2 1 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-7/5/ 0.285 5 1.4 2 1 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-7/6/ 0.31 5 1.4 2 1 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-7/7/ 0.34 5 1.4 2 1 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-7/8/ 0.37 5 1.4 2 1 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-7/9/ 0.40 5 1.4 2 1 1 0 0.8 0.8 2 2 5"
    
    "data/result/L5-SD-8/0/ 0.15 5 1.4 2 2 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-8/1/ 0.163 5 1.4 2 2 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-8/2/ 0.195 5 1.4 2 2 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-8/3/ 0.23 5 1.4 2 2 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-8/4/ 0.255 5 1.4 2 2 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-8/5/ 0.285 5 1.4 2 2 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-8/6/ 0.31 5 1.4 2 2 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-8/7/ 0.34 5 1.4 2 2 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-8/8/ 0.37 5 1.4 2 2 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-8/9/ 0.40 5 1.4 2 2 1 0 0.8 0.8 2 2 5"

    "data/result/L5-SD-9/0/ 0.15 5 1.4 2 3 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-9/1/ 0.163 5 1.4 2 3 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-9/2/ 0.195 5 1.4 2 3 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-9/3/ 0.23 5 1.4 2 3 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-9/4/ 0.255 5 1.4 2 3 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-9/5/ 0.285 5 1.4 2 3 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-9/6/ 0.31 5 1.4 2 3 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-9/7/ 0.34 5 1.4 2 3 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-9/8/ 0.37 5 1.4 2 3 1 0 0.8 0.8 2 2 5"
    "data/result/L5-SD-9/9/ 0.40 5 1.4 2 3 1 0 0.8 0.8 2 2 5"

    # Sensi Ana

    "data/result/L5-YXH/0/ 0.15 5 1.4 0 0 1 0 1 0.8 2 2 5"
    "data/result/L5-YXH/1/ 0.163 5 1.4 0 0 1 0 1 0.8 2 2 5"
    "data/result/L5-YXH/2/ 0.195 5 1.4 0 0 1 0 1 0.8 2 2 5"
    "data/result/L5-YXH/3/ 0.23 5 1.4 0 0 1 0 1 0.8 2 2 5"
    "data/result/L5-YXH/4/ 0.255 5 1.4 0 0 1 0 1 0.8 2 2 5"
    "data/result/L5-YXH/5/ 0.285 5 1.4 0 0 1 0 1 0.8 2 2 5"
    "data/result/L5-YXH/6/ 0.31 5 1.4 0 0 1 0 1 0.8 2 2 5"
    "data/result/L5-YXH/7/ 0.34 5 1.4 0 0 1 0 1 0.8 2 2 5"
    "data/result/L5-YXH/8/ 0.37 5 1.4 0 0 1 0 1 0.8 2 2 5"
    "data/result/L5-YXH/9/ 0.40 5 1.4 0 0 1 0 1 0.8 2 2 5"

    "data/result/L5-YXL/0/ 0.15 5 1.4 0 0 1 0 0.5 0.8 2 2 5"
    "data/result/L5-YXL/1/ 0.163 5 1.4 0 0 1 0 0.5 0.8 2 2 5"
    "data/result/L5-YXL/2/ 0.195 5 1.4 0 0 1 0 0.5 0.8 2 2 5"
    "data/result/L5-YXL/3/ 0.23 5 1.4 0 0 1 0 0.5 0.8 2 2 5"
    "data/result/L5-YXL/4/ 0.255 5 1.4 0 0 1 0 0.5 0.8 2 2 5"
    "data/result/L5-YXL/5/ 0.285 5 1.4 0 0 1 0 0.5 0.8 2 2 5"
    "data/result/L5-YXL/6/ 0.31 5 1.4 0 0 1 0 0.5 0.8 2 2 5"
    "data/result/L5-YXL/7/ 0.34 5 1.4 0 0 1 0 0.5 0.8 2 2 5"
    "data/result/L5-YXL/8/ 0.37 5 1.4 0 0 1 0 0.5 0.8 2 2 5"
    "data/result/L5-YXL/9/ 0.40 5 1.4 0 0 1 0 0.5 0.8 2 2 5"

    "data/result/L5-ZZH/0/ 0.15 5 1.4 0 0 1 0 0.8 1 2 2 5"
    "data/result/L5-ZZH/1/ 0.163 5 1.4 0 0 1 0 0.8 1 2 2 5"
    "data/result/L5-ZZH/2/ 0.195 5 1.4 0 0 1 0 0.8 1 2 2 5"
    "data/result/L5-ZZH/3/ 0.23 5 1.4 0 0 1 0 0.8 1 2 2 5"
    "data/result/L5-ZZH/4/ 0.255 5 1.4 0 0 1 0 0.8 1 2 2 5"
    "data/result/L5-ZZH/5/ 0.285 5 1.4 0 0 1 0 0.8 1 2 2 5"
    "data/result/L5-ZZH/6/ 0.31 5 1.4 0 0 1 0 0.8 1 2 2 5"
    "data/result/L5-ZZH/7/ 0.34 5 1.4 0 0 1 0 0.8 1 2 2 5"
    "data/result/L5-ZZH/8/ 0.37 5 1.4 0 0 1 0 0.8 1 2 2 5"
    "data/result/L5-ZZH/9/ 0.40 5 1.4 0 0 1 0 0.8 1 2 2 5"

    "data/result/L5-ZZL/0/ 0.15 5 1.4 0 0 1 0 0.8 0.5 2 2 5"
    "data/result/L5-ZZL/1/ 0.163 5 1.4 0 0 1 0 0.8 0.5 2 2 5"
    "data/result/L5-ZZL/2/ 0.195 5 1.4 0 0 1 0 0.8 0.5 2 2 5"
    "data/result/L5-ZZL/3/ 0.23 5 1.4 0 0 1 0 0.8 0.5 2 2 5"
    "data/result/L5-ZZL/4/ 0.255 5 1.4 0 0 1 0 0.8 0.5 2 2 5"
    "data/result/L5-ZZL/5/ 0.285 5 1.4 0 0 1 0 0.8 0.5 2 2 5"
    "data/result/L5-ZZL/6/ 0.31 5 1.4 0 0 1 0 0.8 0.5 2 2 5"
    "data/result/L5-ZZL/7/ 0.34 5 1.4 0 0 1 0 0.8 0.5 2 2 5"
    "data/result/L5-ZZL/8/ 0.37 5 1.4 0 0 1 0 0.8 0.5 2 2 5"
    "data/result/L5-ZZL/9/ 0.40 5 1.4 0 0 1 0 0.8 0.5 2 2 5"

    "data/result/L5-Delay21/0/ 0.15 5 1.4 0 0 1 0 0.8 0.8 2 1 5"
    "data/result/L5-Delay21/1/ 0.163 5 1.4 0 0 1 0 0.8 0.8 2 1 5"
    "data/result/L5-Delay21/2/ 0.195 5 1.4 0 0 1 0 0.8 0.8 2 1 5"
    "data/result/L5-Delay21/3/ 0.23 5 1.4 0 0 1 0 0.8 0.8 2 1 5"
    "data/result/L5-Delay21/4/ 0.255 5 1.4 0 0 1 0 0.8 0.8 2 1 5"
    "data/result/L5-Delay21/5/ 0.285 5 1.4 0 0 1 0 0.8 0.8 2 1 5"
    "data/result/L5-Delay21/6/ 0.31 5 1.4 0 0 1 0 0.8 0.8 2 1 5"
    "data/result/L5-Delay21/7/ 0.34 5 1.4 0 0 1 0 0.8 0.8 2 1 5"
    "data/result/L5-Delay21/8/ 0.37 5 1.4 0 0 1 0 0.8 0.8 2 1 5"
    "data/result/L5-Delay21/9/ 0.40 5 1.4 0 0 1 0 0.8 0.8 2 1 5"

    "data/result/L5-Delay24/0/ 0.15 5 1.4 0 0 1 0 0.8 0.8 2 4 5"
    "data/result/L5-Delay24/1/ 0.163 5 1.4 0 0 1 0 0.8 0.8 2 4 5"
    "data/result/L5-Delay24/2/ 0.195 5 1.4 0 0 1 0 0.8 0.8 2 4 5"
    "data/result/L5-Delay24/3/ 0.23 5 1.4 0 0 1 0 0.8 0.8 2 4 5"
    "data/result/L5-Delay24/4/ 0.255 5 1.4 0 0 1 0 0.8 0.8 2 4 5"
    "data/result/L5-Delay24/5/ 0.285 5 1.4 0 0 1 0 0.8 0.8 2 4 5"
    "data/result/L5-Delay24/6/ 0.31 5 1.4 0 0 1 0 0.8 0.8 2 4 5"
    "data/result/L5-Delay24/7/ 0.34 5 1.4 0 0 1 0 0.8 0.8 2 4 5"
    "data/result/L5-Delay24/8/ 0.37 5 1.4 0 0 1 0 0.8 0.8 2 4 5"
    "data/result/L5-Delay24/9/ 0.40 5 1.4 0 0 1 0 0.8 0.8 2 4 5"

    "data/result/L5-Delay12/0/ 0.15 5 1.4 0 0 1 0 0.8 0.8 1 2 5"
    "data/result/L5-Delay12/1/ 0.163 5 1.4 0 0 1 0 0.8 0.8 1 2 5"
    "data/result/L5-Delay12/2/ 0.195 5 1.4 0 0 1 0 0.8 0.8 1 2 5"
    "data/result/L5-Delay12/3/ 0.23 5 1.4 0 0 1 0 0.8 0.8 1 2 5"
    "data/result/L5-Delay12/4/ 0.255 5 1.4 0 0 1 0 0.8 0.8 1 2 5"
    "data/result/L5-Delay12/5/ 0.285 5 1.4 0 0 1 0 0.8 0.8 1 2 5"
    "data/result/L5-Delay12/6/ 0.31 5 1.4 0 0 1 0 0.8 0.8 1 2 5"
    "data/result/L5-Delay12/7/ 0.34 5 1.4 0 0 1 0 0.8 0.8 1 2 5"
    "data/result/L5-Delay12/8/ 0.37 5 1.4 0 0 1 0 0.8 0.8 1 2 5"
    "data/result/L5-Delay12/9/ 0.40 5 1.4 0 0 1 0 0.8 0.8 1 2 5"

    "data/result/L5-Delay42/0/ 0.15 5 1.4 0 0 1 0 0.8 0.8 4 2 5"
    "data/result/L5-Delay42/1/ 0.163 5 1.4 0 0 1 0 0.8 0.8 4 2 5"
    "data/result/L5-Delay42/2/ 0.195 5 1.4 0 0 1 0 0.8 0.8 4 2 5"
    "data/result/L5-Delay42/3/ 0.23 5 1.4 0 0 1 0 0.8 0.8 4 2 5"
    "data/result/L5-Delay42/4/ 0.255 5 1.4 0 0 1 0 0.8 0.8 4 2 5"
    "data/result/L5-Delay42/5/ 0.285 5 1.4 0 0 1 0 0.8 0.8 4 2 5"
    "data/result/L5-Delay42/6/ 0.31 5 1.4 0 0 1 0 0.8 0.8 4 2 5"
    "data/result/L5-Delay42/7/ 0.34 5 1.4 0 0 1 0 0.8 0.8 4 2 5"
    "data/result/L5-Delay42/8/ 0.37 5 1.4 0 0 1 0 0.8 0.8 4 2 5"
    "data/result/L5-Delay42/9/ 0.40 5 1.4 0 0 1 0 0.8 0.8 4 2 5"

    "data/result/L5-TDH/0/ 0.15 5 1.4 0 0 1 0 0.8 0.8 2 2 7"
    "data/result/L5-TDH/1/ 0.163 5 1.4 0 0 1 0 0.8 0.8 2 2 7"
    "data/result/L5-TDH/2/ 0.195 5 1.4 0 0 1 0 0.8 0.8 2 2 7"
    "data/result/L5-TDH/3/ 0.23 5 1.4 0 0 1 0 0.8 0.8 2 2 7"
    "data/result/L5-TDH/4/ 0.255 5 1.4 0 0 1 0 0.8 0.8 2 2 7"
    "data/result/L5-TDH/5/ 0.285 5 1.4 0 0 1 0 0.8 0.8 2 2 7"
    "data/result/L5-TDH/6/ 0.31 5 1.4 0 0 1 0 0.8 0.8 2 2 7"
    "data/result/L5-TDH/7/ 0.34 5 1.4 0 0 1 0 0.8 0.8 2 2 7"
    "data/result/L5-TDH/8/ 0.37 5 1.4 0 0 1 0 0.8 0.8 2 2 7"
    "data/result/L5-TDH/9/ 0.40 5 1.4 0 0 1 0 0.8 0.8 2 2 7"

    "data/result/L5-TDL/0/ 0.15 5 1.4 0 0 1 0 0.8 0.8 2 2 3"
    "data/result/L5-TDL/1/ 0.163 5 1.4 0 0 1 0 0.8 0.8 2 2 3"
    "data/result/L5-TDL/2/ 0.195 5 1.4 0 0 1 0 0.8 0.8 2 2 3"
    "data/result/L5-TDL/3/ 0.23 5 1.4 0 0 1 0 0.8 0.8 2 2 3"
    "data/result/L5-TDL/4/ 0.255 5 1.4 0 0 1 0 0.8 0.8 2 2 3"
    "data/result/L5-TDL/5/ 0.285 5 1.4 0 0 1 0 0.8 0.8 2 2 3"
    "data/result/L5-TDL/6/ 0.31 5 1.4 0 0 1 0 0.8 0.8 2 2 3"
    "data/result/L5-TDL/7/ 0.34 5 1.4 0 0 1 0 0.8 0.8 2 2 3"
    "data/result/L5-TDL/8/ 0.37 5 1.4 0 0 1 0 0.8 0.8 2 2 3"
    "data/result/L5-TDL/9/ 0.40 5 1.4 0 0 1 0 0.8 0.8 2 2 3"

    "data/result/L0-Tem/0/ 0.15 0 1.4 0 0 0 0 0.8 0.8 2 2 5"
    "data/result/L0-Tem/1/ 0.163 0 1.4 0 0 0 0 0.8 0.8 2 2 5"
    "data/result/L0-Tem/2/ 0.195 0 1.4 0 0 0 0 0.8 0.8 2 2 5"
    "data/result/L0-Tem/3/ 0.23 0 1.4 0 0 0 0 0.8 0.8 2 2 5"
    "data/result/L0-Tem/4/ 0.255 0 1.4 0 0 0 0 0.8 0.8 2 2 5"
    "data/result/L0-Tem/5/ 0.285 0 1.4 0 0 0 0 0.8 0.8 2 2 5"
    "data/result/L0-Tem/6/ 0.31 0 1.4 0 0 0 0 0.8 0.8 2 2 5"
    "data/result/L0-Tem/7/ 0.34 0 1.4 0 0 0 0 0.8 0.8 2 2 5"
    "data/result/L0-Tem/8/ 0.37 0 1.4 0 0 0 0 0.8 0.8 2 2 5"
    "data/result/L0-Tem/9/ 0.40 0 1.4 0 0 0 0 0.8 0.8 2 2 5"

    "data/result/L0-P0/0/ 0.15 0 1.4 0 0 1 1 0.8 0.8 2 2 5"
    "data/result/L0-P0/1/ 0.163 0 1.4 0 0 1 1 0.8 0.8 2 2 5"
    "data/result/L0-P0/2/ 0.195 0 1.4 0 0 1 1 0.8 0.8 2 2 5"
    "data/result/L0-P0/3/ 0.23 0 1.4 0 0 1 1 0.8 0.8 2 2 5"
    "data/result/L0-P0/4/ 0.255 0 1.4 0 0 1 1 0.8 0.8 2 2 5"
    "data/result/L0-P0/5/ 0.285 0 1.4 0 0 1 1 0.8 0.8 2 2 5"
    "data/result/L0-P0/6/ 0.31 0 1.4 0 0 1 1 0.8 0.8 2 2 5"
    "data/result/L0-P0/7/ 0.34 0 1.4 0 0 1 1 0.8 0.8 2 2 5"
    "data/result/L0-P0/8/ 0.37 0 1.4 0 0 1 1 0.8 0.8 2 2 5"
    "data/result/L0-P0/9/ 0.40 0 1.4 0 0 1 1 0.8 0.8 2 2 5"

    "data/result/L0-P100/0/ 0.15 0 1.4 0 0 1 2 0.8 0.8 2 2 5"
    "data/result/L0-P100/1/ 0.163 0 1.4 0 0 1 2 0.8 0.8 2 2 5"
    "data/result/L0-P100/2/ 0.195 0 1.4 0 0 1 2 0.8 0.8 2 2 5"
    "data/result/L0-P100/3/ 0.23 0 1.4 0 0 1 2 0.8 0.8 2 2 5"
    "data/result/L0-P100/4/ 0.255 0 1.4 0 0 1 2 0.8 0.8 2 2 5"
    "data/result/L0-P100/5/ 0.285 0 1.4 0 0 1 2 0.8 0.8 2 2 5"
    "data/result/L0-P100/6/ 0.31 0 1.4 0 0 1 2 0.8 0.8 2 2 5"
    "data/result/L0-P100/7/ 0.34 0 1.4 0 0 1 2 0.8 0.8 2 2 5"
    "data/result/L0-P100/8/ 0.37 0 1.4 0 0 1 2 0.8 0.8 2 2 5"
    "data/result/L0-P100/9/ 0.40 0 1.4 0 0 1 2 0.8 0.8 2 2 5"

    "data/result/L5-Tem/0/ 0.15 5 1.4 0 0 0 0 0.8 0.8 2 2 5"
    "data/result/L5-Tem/1/ 0.163 5 1.4 0 0 0 0 0.8 0.8 2 2 5"
    "data/result/L5-Tem/2/ 0.195 5 1.4 0 0 0 0 0.8 0.8 2 2 5"
    "data/result/L5-Tem/3/ 0.23 5 1.4 0 0 0 0 0.8 0.8 2 2 5"
    "data/result/L5-Tem/4/ 0.255 5 1.4 0 0 0 0 0.8 0.8 2 2 5"
    "data/result/L5-Tem/5/ 0.285 5 1.4 0 0 0 0 0.8 0.8 2 2 5"
    "data/result/L5-Tem/6/ 0.31 5 1.4 0 0 0 0 0.8 0.8 2 2 5"
    "data/result/L5-Tem/7/ 0.34 5 1.4 0 0 0 0 0.8 0.8 2 2 5"
    "data/result/L5-Tem/8/ 0.37 5 1.4 0 0 0 0 0.8 0.8 2 2 5"
    "data/result/L5-Tem/9/ 0.40 5 1.4 0 0 0 0 0.8 0.8 2 2 5"

    "data/result/L5-P0/0/ 0.15 5 1.4 0 0 1 1 0.8 0.8 2 2 5"
    "data/result/L5-P0/1/ 0.163 5 1.4 0 0 1 1 0.8 0.8 2 2 5"
    "data/result/L5-P0/2/ 0.195 5 1.4 0 0 1 1 0.8 0.8 2 2 5"
    "data/result/L5-P0/3/ 0.23 5 1.4 0 0 1 1 0.8 0.8 2 2 5"
    "data/result/L5-P0/4/ 0.255 5 1.4 0 0 1 1 0.8 0.8 2 2 5"
    "data/result/L5-P0/5/ 0.285 5 1.4 0 0 1 1 0.8 0.8 2 2 5"
    "data/result/L5-P0/6/ 0.31 5 1.4 0 0 1 1 0.8 0.8 2 2 5"
    "data/result/L5-P0/7/ 0.34 5 1.4 0 0 1 1 0.8 0.8 2 2 5"
    "data/result/L5-P0/8/ 0.37 5 1.4 0 0 1 1 0.8 0.8 2 2 5"
    "data/result/L5-P0/9/ 0.40 5 1.4 0 0 1 1 0.8 0.8 2 2 5"

    "data/result/L5-P100/0/ 0.15 5 1.4 0 0 1 2 0.8 0.8 2 2 5"
    "data/result/L5-P100/1/ 0.163 5 1.4 0 0 1 2 0.8 0.8 2 2 5"
    "data/result/L5-P100/2/ 0.195 5 1.4 0 0 1 2 0.8 0.8 2 2 5"
    "data/result/L5-P100/3/ 0.23 5 1.4 0 0 1 2 0.8 0.8 2 2 5"
    "data/result/L5-P100/4/ 0.255 5 1.4 0 0 1 2 0.8 0.8 2 2 5"
    "data/result/L5-P100/5/ 0.285 5 1.4 0 0 1 2 0.8 0.8 2 2 5"
    "data/result/L5-P100/6/ 0.31 5 1.4 0 0 1 2 0.8 0.8 2 2 5"
    "data/result/L5-P100/7/ 0.34 5 1.4 0 0 1 2 0.8 0.8 2 2 5"
    "data/result/L5-P100/8/ 0.37 5 1.4 0 0 1 2 0.8 0.8 2 2 5"
    "data/result/L5-P100/9/ 0.40 5 1.4 0 0 1 2 0.8 0.8 2 2 5"

)
for ((i=0; i<${#groups[@]}; i+=2)); do
    group1="${groups[$i]}"
    group2="${groups[$i+1]}"
    
    echo "Run $group1 and $group2"
    
    for arg in "${args[@]}"; do
        if [[ $arg == *"/result/$group1/"* ]]; then
            nohup ./myprogram $arg &> /dev/null &
        fi
    done
    
    if [[ -n $group2 ]]; then
        for arg in "${args[@]}"; do
            if [[ $arg == *"/result/$group2/"* ]]; then
                nohup ./myprogram $arg &> /dev/null &
            fi
        done
    fi
    
    wait
    
    echo " $group1 and $group2 done"

done

echo "All done"