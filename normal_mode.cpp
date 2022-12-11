//
// Created by Lenovo on 2022/12/11.
//

#include <ncurses.h>
#include "minivim.h"
#include <string>
#include <vector>

extern int cursor_x, cursor_y,col;
extern const int filel=10;
extern WINDOW *file_win, *info_win, *comm_win;
extern std::string filename;
extern std::vector<std::string> tdata;

void normal_mode(){
    int ch;
    while ( 1 ) {
        getyx(file_win, cursor_y, cursor_x);
        ch=getch();
        switch (ch) {
            case KEY_UP:{cursor_y--;
                if(cursor_y>=0)wmove(file_win, cursor_y, cursor_x);
                break;}
            case KEY_DOWN:{cursor_y++;
                if(cursor_y<=filel-1)wmove(file_win, cursor_y, cursor_x);
                break;}
            case KEY_LEFT:{cursor_x--;
                if (cursor_x >= 0)wmove(file_win, cursor_y, cursor_x);
                break;}
            case KEY_RIGHT:{cursor_x++;
                if (cursor_x <= col)wmove(file_win, cursor_y, cursor_x);
                break;}
            case 58:{
                mvprintw(filel+1,0, ":");
                command_mode();
                break;}//冒号切换模式
            case 111:{cursor_x=0;
                wmove(file_win, cursor_y, cursor_x);
                break;}//o回到行开头
            case 100:if ((ch=getch())==100)tdata.erase(tdata.begin()+cursor_y);break;//dd键删掉当前一整行
            case 36:cursor_x=tdata[cursor_y].size()-1;
                wmove(file_win,cursor_y,cursor_x);
                break;
            case 105:
                insert_mode();//输入i切换模式
        }
        wmove(info_win, 0, 0);
        wprintw(info_win,"normal mode %d %d %s",cursor_y,cursor_x,filename.c_str());//在info_win中输出模式、文件名、光标行列等信息

        refresh();
    }
}
