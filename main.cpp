#include "common.h"

int main() {

    std::cout << " _____ _               _____ _               _             \n"
    "/  ___| |             /  __ \\ |             | |            \n"
    "\\ `--.| |_ ___ _ __   | /  \\/ |__   ___  ___| | _____ _ __ \n"
    " `--. \\ __/ _ \\ '_ \\  | |   | '_ \\ / _ \\/ __| |/ / _ \\ '__|\n"
    "/\\__/ / ||  __/ |_) | | \\__/\\ | | |  __/ (__|   <  __/ |   \n"
    "\\____/ \\__\\___| .__/   \\____/_| |_|\\___|\\___|_|\\_\\___|_|   \n"
    "              | |                                          \n"
    "              |_|                                          \n"
    ""; 
    std::cout << "Welcome to Step Checker, a sandbox tester that uses common malware techniques!\n";
    std::cout << "[*] UI Opened\n";
    std::cout << "If there is an asterisk in between the brackets, that means it is ok!\n";
    
    std::cout << "\n_____________________________[Human Interaction]_____________________________\n";

    std::vector<int> interaction_ui;
    init_interaction();

    if (check_mouse_presence()) {

        interaction_ui.push_back(1);

        if (check_mouse_movement()) {

            interaction_ui.push_back(1);

            if (check_mouse_speed()) {

                interaction_ui.push_back(1);

                if (check_mouse_click()) {

                    interaction_ui.push_back(1);

                } else {

                    interaction_ui.push_back(0);

                }
            } else {

                interaction_ui.push_back(0);

                if (check_mouse_click()) {

                    interaction_ui.push_back(1);

                } else {

                    interaction_ui.push_back(0);

                }

            }
        } else {

            interaction_ui.push_back(0);
            interaction_ui.push_back(-1);

            if (check_mouse_click()) {

                interaction_ui.push_back(1);

            } else {

                interaction_ui.push_back(0);

            }
        }
    } else {

        interaction_ui.push_back(0);
        interaction_ui.push_back(-1);
        interaction_ui.push_back(-1);
        interaction_ui.push_back(-1);

    }
    if (check_dialog_confirmation()) {

        interaction_ui.push_back(1);

    } else {

        interaction_ui.push_back(0);

    }

    switch (interaction_ui[0]) {
        case 1:
            std::cout << "[*] Mouse Presence\n";
            break;

        case 0:
            std::cout << "[ ] Mouse Presence\n";
            break;
    }

    switch (interaction_ui[1]) {
        case 1: 
            std::cout << "[*] Mouse Movement\n";
            break;
        
        case 0: 
            std::cout << "[ ] Mouse Movement\n";
            break;

        case -1:
            std::cout << "[/] Mouse Movement\n";
            break;
    }

    switch (interaction_ui[2]) {
        case 1:
            std::cout << "[*] Mouse Speed\n";
            break;

        case 0:
            std::cout << "[ ] Mouse Speed\n";
            break;

        case -1:
            std::cout << "[/] Mouse Speed\n";
            break;
    }

    switch (interaction_ui[3]) {
        case 1:
            std::cout << "[*] Mouse Click\n";
            break;

        case 0:
            std::cout << "[ ] Mouse Click\n";
            break;

        case -1:
            std::cout << "[/] Mouse Click\n";
            break;
    }

    switch (interaction_ui[4]) {
        case 1:
            std::cout << "[*] Dialog Confirmation\n";
            break;
        
        case 0:
            std::cout << "[ ] Dialog Confirmation\n";
            break;
    }
    
/*  Previous UI method:
    if (check_mouse_presence()) {

        std::cout << "[*] Mouse Presence\n";

        if (check_mouse_movement()) {

            std::cout << "[*] Mouse Movement\n";

            if (check_mouse_speed()) {

                std::cout << "[*] Mouse Speed\n";

                if (check_mouse_click()) {

                    std::cout << "[*] Mouse Click\n";

                } else {

                    std::cout << "[ ] Mouse Click\n";

                }

            } else {

                std::cout << "[ ] Mouse Speed\n";

                if (check_mouse_click()) {

                    std::cout << "[*] Mouse Click\n";

                } else {

                    std::cout << "[ ] Mouse Click\n";

                }

            }

        } else {

            std::cout << "[ ] Mouse Movement\n";
            std::cout << "[/] Mouse Speed\n";

            if (check_mouse_click()) {

                std::cout << "[*] Mouse Click\n";
                
            } else {

                std::cout << "[ ] Mouse Click\n";

            }

        }
    } else {

        std::cout << "[ ] Mouse Presence\n";
        std::cout << "[/] Mouse Movement\n";
        std::cout << "[/] Mouse Speed\n";
        std::cout << "[/] Mouse Click\n";

    } */

    std::cout << "\n______________________________[VMWare Detection]______________________________\n";
    std::vector<int> vmware_ui;
    init_vmware();

    if (check_processes_running()) {
        vmware_ui.push_back(1);
    } else {
        vmware_ui.push_back(0);
    }
    if (check_vm_processes()) {
        vmware_ui.push_back(1);
    } else {
        vmware_ui.push_back(0);
    }
    if (check_vbox_regkeys()) {
        vmware_ui.push_back(1);
    } else {
        vmware_ui.push_back(0);
    }
    if (check_vbox_sysfiles()) {
        vmware_ui.push_back(1);
    } else {
        vmware_ui.push_back(0);
    }
    if (check_vbox_mac()) {
        vmware_ui.push_back(1);
    } else {
        vmware_ui.push_back(0);
    }
    if (check_vbox_wmi()) {
        vmware_ui.push_back(1);
    } else {
        vmware_ui.push_back(0);
    }
    if (check_vmware_regkeys()) {
        vmware_ui.push_back(1);
    } else {
        vmware_ui.push_back(0);
    }
    if (check_vmware_sysfiles()) {
        vmware_ui.push_back(1);
    } else {
        vmware_ui.push_back(0);
    }
    if (check_vmware_mac()) {
        vmware_ui.push_back(1);
    } else {
        vmware_ui.push_back(0);
    }
    if (check_vmware_wmi()) {
        vmware_ui.push_back(1);
    } else {
        vmware_ui.push_back(0);
    }

    switch (vmware_ui[0]) {
        case 1:
            std::cout << "[*] Processes Running\n";
            break;

        case 0:
            std::cout << "[ ] Processes Running\n";
            break;
    }

    switch (vmware_ui[1]) {
        case 1:
            std::cout << "[*] VMWare Processes\n";
            break;
        
        case 0:
            std::cout << "[ ] VMWare Processes\n";
            break;
    }

    switch (vmware_ui[2]) {
        case 1:
            std::cout << "[*] VBox Regkeys\n";
            break;
        
        case 0:
            std::cout << "[ ] VBox Regkeys\n";
            break;
    }

    switch (vmware_ui[3]) {
        case 1:
            std::cout << "[*] VBox Sysfiles\n";
            break;
        
        case 0:
            std::cout << "[ ] VBox Sysfiles\n";
            break;
    }

    switch (vmware_ui[4]) {
        case 1:
            std::cout << "[*] VBox Mac\n";
            break;
        
        case 0:
            std::cout << "[ ] VBox Mac\n";
            break;
    }

    switch (vmware_ui[5]) {
        case 1:
            std::cout << "[*] VBox WMI\n";
            break;
        
        case 0:
            std::cout << "[ ] VBox WMI\n";
            break;
    }

    switch (vmware_ui[6]) {
        case 1:
            std::cout << "[*] VMWare Regkeys\n";
            break;
        
        case 0:
            std::cout << "[ ] VMWare Regkeys\n";
            break;
    }

    switch (vmware_ui[7]) {
        case 1:
            std::cout << "[*] VMWare Sysfiles\n";
            break;
        
        case 0:
            std::cout << "[ ] VMWare Sysfiles\n";
            break;
    }

    switch (vmware_ui[8]) {
        case 1:
            std::cout << "[*] VMWare Mac\n";
            break;
        
        case 0:
            std::cout << "[ ] VMWare Mac\n";
            break;
    }

    switch (vmware_ui[9]) {
        case 1:
            std::cout << "[*] VMWare WMI\n";
            break;
        
        case 0:
            std::cout << "[ ] VMWare WMI\n";
            break;
    }

    std::cout << "\n_____________________________[Hardware Detection]_____________________________\n";
    std::vector<int> hardware_ui;

    if (check_cores()) {
        hardware_ui.push_back(1);
    } else {
        hardware_ui.push_back(0);
    }
    if (check_disk_size()) {
        hardware_ui.push_back(1);
    } else {
        hardware_ui.push_back(0);
    }
    if (check_memory_space()) {
        hardware_ui.push_back(1);
    } else {
        hardware_ui.push_back(0);
    }
    if (check_model()) {
        hardware_ui.push_back(1);
    } else {
        hardware_ui.push_back(0);
    }
    if (check_manufacturer()) {
        hardware_ui.push_back(1);
    } else {
        hardware_ui.push_back(0);
    }
    if (check_win32_entries()) {
        hardware_ui.push_back(1);
    } else {
        hardware_ui.push_back(0);
    }
    if(check_CIM_entries()) {
        hardware_ui.push_back(1);
    } else {
        hardware_ui.push_back(0);
    }

    switch (hardware_ui[0]) {
        case 1:
            std::cout << "[*] Realistic Number of Cores\n";
            break;

        case 0:
            std::cout << "[ ] Realistic Number of Core\n";
            break;
    }
    switch (hardware_ui[1]) {
        case 1:
            std::cout << "[*] Realistic Hard Disk Size\n";
            break;

        case 0:
            std::cout << "[ ] Realistic Hard Disk Size\n";
            break;
    }
    switch (hardware_ui[2]) {
        case 1:
            std::cout << "[*] Realistic Memory Space\n";
            break;

        case 0:
            std::cout << "[ ] Realistic Memory Space\n";
            break;
    }
    switch (hardware_ui[3]) {
        case 1:
            std::cout << "[*] Real Models\n";
            break;

        case 0:
            std::cout << "[ ] Real Models\n";
            break;
    }
    switch (hardware_ui[4]) {
        case 1:
            std::cout << "[*] Real Manufacturers\n";
            break;

        case 0:
            std::cout << "[ ] Real Manufacturers\n";
            break;
    }
    switch (hardware_ui[5]) {
        case 1:
            std::cout << "[*] Win32 Entries\n";
            break;

        case 0:
            std::cout << "[ ] Win32 Entries\n";
            break;
    }
    switch (hardware_ui[6]) {
        case 1:
            std::cout << "[*] CIM Entries\n";
            break;

        case 0:
            std::cout << "[ ] CIM Entries\n";
            break;
    }

    std::cout << "\n___________________________________[Timing]___________________________________\n";
    std::vector<int> timing_ui;

    //Timing UI is changed so that you can see what is the next timing attack being used, and for easier bug testing

    if (ntDelay()) {
        timing_ui.push_back(1);
    } else {
        timing_ui.push_back(0);
    }
    switch (timing_ui[0]) {
        case 1:
            std::cout << "[*] NT Delay Working\n";
            break;

        case 0:
            std::cout << "[ ] NT Delay Working\n";
            break;
    }
    if (sleepLoop()) {
        timing_ui.push_back(1);
    } else {
        timing_ui.push_back(0);
    }
    switch (timing_ui[1]) {
        case 1:
            std::cout << "[*] Sleep Loop Working\n";
            break;

        case 0:
            std::cout << "[ ] Sleep Loop Working\n";
            break;
    }
    if (rdtscDiff()) {
        timing_ui.push_back(1);
    } else {
        timing_ui.push_back(0);
    }
    switch (timing_ui[2]) {
        case 1:
            std::cout << "[*] RDTSC Diff\n";
            break;

        case 0:
            std::cout << "[ ] RDTSC Diff\n";
            break;
    }
    if (rdtscCPUID()) {
        timing_ui.push_back(1);
    } else {
        timing_ui.push_back(0);
    }
    switch (timing_ui[3]) {
        case 1:
            std::cout << "[*] RDTSC CPUID\n";
            break;

        case 0:
            std::cout << "[ ] RDTSC CPUID\n";
            break;
    }
    if (icmpEcho()) {
        timing_ui.push_back(1);
    } else {
        timing_ui.push_back(0);
    }
    switch (timing_ui[4]) {
        case 1:
            std::cout << "[*] ICMP Echo\n";
            break;

        case 0:
            std::cout << "[ ] ICMP Echo\n";
            break;
    }
    if (waitableTimer()) {
        timing_ui.push_back(1);
    } else {
        timing_ui.push_back(0);
    }
    switch (timing_ui[5]) {
        case 1:
            std::cout << "[*] Waitable Timer\n";
            break;

        case 0:
            std::cout << "[ ] Waitable Timer\n";
            break;
    }
    if (queueTimer()) {
        timing_ui.push_back(1);
    } else {
        timing_ui.push_back(0);
    }
    switch (timing_ui[6]) {
        case 1:
            std::cout << "[*] Queue Timer\n";
            break;

        case 0:
            std::cout << "[ ] Queue Timer\n";
            break;
    }

    std::cout << "\n__________________________________[Analysis]__________________________________\n";
    std::vector<int> analysis_ui;

    /// Uses built in debugger check from debug api, no need for separate function
    if (!IsDebuggerPresent()) {
        analysis_ui.push_back(1);
    } else {
        analysis_ui.push_back(0);
    }
    if (remoteDebugger()) {
        analysis_ui.push_back(1);
    } else {
        analysis_ui.push_back(0);
    }
    if(hardwareBreakpoints()) {
        analysis_ui.push_back(1);
    } else {
        analysis_ui.push_back(0);
    }
    if (lfhCheck() == 1) {
        analysis_ui.push_back(1);
    } else if (lfhCheck() == 0) {
        analysis_ui.push_back(0);
    } else if (lfhCheck() == -1) {
        analysis_ui.push_back(-1);
    }
    if (outputString()) {
        analysis_ui.push_back(1);
    } else {
        analysis_ui.push_back(0);
    }
    if (tools()) {
        analysis_ui.push_back(1);
    } else {
        analysis_ui.push_back(0);
    }
    if (eraseHeader()) {
        analysis_ui.push_back(1);
    } else {
        analysis_ui.push_back(0);
    }
    if (headerImageSize()) {
        analysis_ui.push_back(1);
    } else {
        analysis_ui.push_back(0);
    }

    switch (analysis_ui[0]) {
        case 1:
            std::cout << "[*] No Debugger\n";
            break;
        
        case 0:
            std::cout << "[ ] No Debugger\n";
            break;
    }
    switch (analysis_ui[1]) {
        case 1:
            std::cout << "[*] No Remote Debugger\n";
            break;

        case 0:
            std::cout << "[ ] No Remote Debugger\n";
            break;
    }
    switch (analysis_ui[2]) {
        case 1:
            std::cout << "[*] No Hardware Breakpoint\n";
            break;
        
        case 0:
            std::cout << "[ ] No Hardware Breakpoint\n";
            break;
    }
    switch (analysis_ui[3]) {
        case 1:
            std::cout << "[*] LFH Check\n";
            break;
        
        case 0:
            std::cout << "[ ] LFH Check\n";
            break;

        case -1: 
            std::cout << "[/] LFH Check\n";
    }
    switch (analysis_ui[4]) {
        case 1:
            std::cout << "[*] No String Error\n";
            break;
        
        case 0:
            std::cout << "[ ] No String Error\n";
            break;
    }
    switch (analysis_ui[5]) {
        case 1:
            std::cout << "[*] No Analysis Tools\n";
            break;
        
        case 0:
            std::cout << "[ ] No Analysis Tools\n";
            break;
    }
    switch (analysis_ui[6]) {
        case 1:
            std::cout << "[*] Erased Header\n";
            break;
        
        case 0:
            std::cout << "[ ] Erased Header\n";
            break;
    }
    switch (analysis_ui[7]) {
        case 1:
            std::cout << "[*] Increased Header Image Size\n";
            break;
        
        case 0:
            std::cout << "[ ] Increased Header Image Size\n";
            break;
    }

    std::cout << "\n\nYou're done! Congratulations! (If you didn't get any flags anyway)";
}