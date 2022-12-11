//
// Created by Lenovo on 2022/12/11.
//
#include <ncurses.h>
#include "minivim.h"
#include <vector>
#include <string>

extern int cursor_x, cursor_y,col;
extern const int filel;
extern WINDOW *file_win, *info_win, *comm_win;
extern std::string filename;
extern std::vector<std::string> tdata;

void freshdata(std::vector<std::string> data);
void insert_mode(){
    int ch;

    tdata.push_back("");
    getyx(file_win, cursor_y, cursor_x);
    ch = getch();
    switch (ch) {
        case KEY_UP:{cursor_y--;
            if(cursor_y>=0)wmove(file_win, cursor_y, cursor_x);
            break;}
        case KEY_DOWN:{cursor_y++;
            if(cursor_y<=filel)wmove(file_win, cursor_y, cursor_x);
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
        default:{
            if(cursor_x==tdata[cursor_y].size())tdata[cursor_y].push_back(char(ch));
            else tdata[cursor_y]=tdata[cursor_y].substr(0,cursor_x)+char(ch)+tdata[cursor_y].substr(cursor_x,tdata[cursor_y].size()-cursor_x);
            cursor_x++;
            if(cursor_x>col){cursor_x=0;cursor_y++;}
            move(cursor_y,cursor_x);
            waddch(file_win,ch);
            freshdata(tdata);
        }
    }

    wprintw(info_win,"normal mode %d %d %s",cursor_y,cursor_x,filename.c_str());
    refresh();
}
void freshdata(std::vector<std::string> data){
    if(data.size()<=filel)
        for(int i=0;i<data.size();i++){
            move(i,0);
            wprintw(file_win,"%s",data[i].c_str());
        }
    else if(cursor_y==filel)
        for(int i=0;i<filel;i++){
            move(i,0);
            wprintw(file_win,"%s",data[data.size()+i-filel-1].c_str());
        }

}