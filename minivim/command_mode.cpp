//
// Created by Lenovo on 2022/12/11.
//
#include <ncurses.h>
#include "minivim.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

extern int cursor_x, cursor_y,col;
extern WINDOW *file_win, *info_win, *comm_win;
extern std::vector<std::string> tdata;
extern std::string filename;

void command_mode(){
    static std::vector<std::string>history_comm;
    std::vector<std::string>commands=history_comm;
    commands.push_back("");
    int order=commands.size()-1;

    int ch;
    getyx(comm_win, cursor_y, cursor_x);
    switch(ch=getch()){
        case 27:normal_mode();
            werase(comm_win);
            wrefresh(comm_win);
            break;//退出command mode
        case KEY_LEFT:{cursor_x--;
            if (cursor_x >= 0)wmove(comm_win, cursor_y, cursor_x);
            break;}
        case KEY_RIGHT:{cursor_x++;
            if (cursor_x <= col)wmove(comm_win, cursor_y, cursor_x);
            break;}
        case KEY_BACKSPACE:{mvwdelch(file_win, cursor_y, cursor_x);
            commands[order].erase(commands[order].begin()+cursor_x);
            break;}
        case KEY_UP:
            if(order>0){order--;wprintw(comm_win,"%s",commands[order].c_str());}
        case KEY_DOWN:
            if(order<commands.size()){order++;wprintw(comm_win,"%s",commands[order].c_str());}
        case 13:
            if(commands[order]=="wq"){std::ofstream oitfile;
                oitfile.open(filename.c_str());
                for(int i=0;i<tdata.size();i++)
                    oitfile<<tdata[i]<<std::endl;
                oitfile.close();
                endwin();}
            if(commands[order]=="w"){std::ofstream oitfile;
                oitfile.open(filename.c_str());
                for(int i=0;i<tdata.size();i++)
                    oitfile<<tdata[i]<<std::endl;
                oitfile.close();}
            if(commands[order]=="q")wprintw(comm_win,"WARNING!!This file is changed and unsaved!Please add ! to override.");
            if(commands[order]=="q!")endwin();
            history_comm.push_back(commands[order]);
            break;
        default:{waddch(comm_win,ch);
            wmove(comm_win,cursor_y,cursor_x);
            commands[order].insert(commands[order].begin()+cursor_x,char(ch));
            refresh();
        }
        refresh();
        wmove(info_win, 0, 0);
        wprintw(info_win,"normal mode %d %d %s",cursor_y,cursor_x,filename.c_str());
    }
}
