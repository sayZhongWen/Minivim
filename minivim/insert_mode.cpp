//
// Created by Lenovo on 2022/12/11.
//
#include <ncurses.h>
#include "minivim.h"
#include <vector>
#include <string>

extern int cursor_x, cursor_y,col;
extern WINDOW *file_win, *info_win, *comm_win;
extern std::string filename;

void freshdata(std::vector<std::string> data);
void insert_mode(){
    int ch;
    std::vector<std::string> tdata;
    getyx(file_win, cursor_y, cursor_x);
    ch = getch();
    scrollok(file_win, true);//滚动窗口
    switch (ch) {
        case KEY_UP:{cursor_y--;
            wmove(file_win, cursor_y, cursor_x);
            break;}
        case KEY_DOWN:{cursor_y++;
            wmove(file_win, cursor_y, cursor_x);
            break;}
        case KEY_LEFT:{cursor_x--;
            if (cursor_x >= 0)wmove(file_win, cursor_y, cursor_x);
            break;}
        case KEY_RIGHT:{cursor_x++;
            if (cursor_x <= col)wmove(file_win, cursor_y, cursor_x);
            break;}
        case 9:for(int i=0;i<4;i++)
            tdata[cursor_y].insert(tdata[cursor_y].begin()+cursor_x,' ');
            freshdata(tdata);//tab?
        case 32:cursor_x++;
            if(cursor_x>col){cursor_x=0;cursor_y++;}
            move(cursor_y,cursor_x);
            waddch(file_win,' ');
            tdata[cursor_y].insert(tdata[cursor_y].begin()+cursor_x,' ');
        case KEY_BACKSPACE:{mvwdelch(file_win, cursor_y, cursor_x);
            tdata[cursor_y].erase(tdata[cursor_y].begin()+cursor_x);
            break;}//删除
        case 13:{
            if(cursor_x==tdata[cursor_y].size())
                tdata.insert(tdata.begin()+cursor_y,"");
            else {std::string temp;
                temp=tdata[cursor_y];
                tdata.insert(tdata.begin()+cursor_y,"");
                temp.erase(tdata[cursor_y].begin()+cursor_x,tdata[cursor_y].begin()+tdata[cursor_y].size());
                tdata[cursor_y]=temp;
                tdata[cursor_y+1].erase(tdata[cursor_y].begin(),tdata[cursor_y].begin()+cursor_x);
                }//换行
            cursor_y++;
            cursor_x=0;
            wmove(file_win, cursor_y, cursor_x);
            freshdata(tdata);
        }//回车
        case 27:normal_mode();break;//退出insert mode
        default:{cursor_x++;
            if(cursor_x>col){cursor_x=0;cursor_y++;}
            move(cursor_y,cursor_x);
            waddch(file_win,ch);
            tdata[cursor_y].insert(tdata[cursor_y].begin()+cursor_x,char(ch));
            freshdata(tdata);
        }
    }

    wprintw(info_win,"normal mode %d %d %s",cursor_y,cursor_x,filename.c_str());
    refresh();
}
void freshdata(std::vector<std::string> data){
    for(int i=0;i<data.size();i++){
        wprintw(file_win,"%s",data[i].c_str());
    }
}