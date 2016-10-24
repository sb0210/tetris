#include<FL/Fl.H>
#include<FL/Fl_Window.H>
#include<FL/Fl_Box.H>
#include<FL/fl_draw.H>

#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include<sstream>

using namespace std;

static const int tilesize = 30;
static const int xmaxtiles= 14;
static const int ymaxtiles= 20;
static const int bgcolor = 97;
static const double timeout=0.3;
static const int boardcolor=0;



class Tile{
	public:
	int x;
	int y;
};
class Board;
class ActivePiece{//Active pieces.
	Tile tileset[4];
	int color;
	int midpointtile;
	friend class Board;
	Board *b;
   	ActivePiece(Board *container);
   	void left();//moves pieces left
	void right();//moves pieces right
	void down();//moves pieces down
	void rotate(int direction);//rotates pieces
   	bool moveDown();//check whether piece can move down or not.
	bool moveLeft();//check whether piece can move left or not.
	bool moveRight();//check whether piece can move right or not.
	bool rotat(int direction);//check whether piece can rotate or not.
};

class Board : public Fl_Widget{
	int occup[xmaxtiles][ymaxtiles];
	ActivePiece* p;//creating the activ piece
	ActivePiece* nextpiece;//the next piece
	char* scoreLabel;//for labeling score card
	Fl_Box *scoreBox;//scorebox
	int score;//counting score
	int level;//counting level (level=score/10 +1)
	public:
	int prepiececolor;//to just ensure that no two consecutive tiles have same color
	Board(int bx,int by,int bw,int bh);//Constructor
	void draw();//draw
	int periodic();//to periodically move the piece down
	void setScoreBox(Fl_Box **b);//setting the score
	bool isFree(Tile ts[4]);//check that the tiles are free or not.
	bool inBoard(Tile ts[4]);//check that the piece is in board or not
	bool transferTiles(Tile ts[4],int color);//transfering tiles to the board
	void deleteRow(int row);//deleting the row.
	bool rowDone(int row);//checking whether a row is filled at this instance
	bool gameover();//gameOver, when we cant create new piece
	int handle(int e);//handling all the events.
};

void timeractions(void *p);//for periodically repetition.

ActivePiece :: ActivePiece(Board *container){
    b=container;
    int r = rand()%7;
    int c = (int)xmaxtiles/2;
	color = rand()%15;
	while(color==boardcolor||color==0||b->prepiececolor==color)color = rand()%15;
    switch(r){
        case 0:
            tileset[0].x=c-2;tileset[0].y=0;//horizontal bar
            tileset[1].x=c-1;tileset[1].y=0;
            tileset[2].x=c;tileset[2].y=0;
            tileset[3].x=c+1;tileset[3].y=0;
            midpointtile=2;
            break;
        case 1:
            tileset[0].x=c-1;tileset[0].y=0;//'-,
            tileset[1].x=c-1;tileset[1].y=1;
            tileset[2].x=c;tileset[2].y=1;
            tileset[3].x=c;tileset[3].y=2;
            midpointtile=2;
            break;
        case 2:
            tileset[0].x=c;tileset[0].y=0;//,-'
            tileset[1].x=c;tileset[1].y=1;
            tileset[2].x=c-1;tileset[2].y=1;
            tileset[3].x=c-1;tileset[3].y=2;
            midpointtile=2;
            break;
        case 3:
            tileset[0].x=c-1;tileset[0].y=0;//box
            tileset[1].x=c-1;tileset[1].y=1;
            tileset[2].x=c;tileset[2].y=1;
            tileset[3].x=c;tileset[3].y=0;
            midpointtile=4;
            break;
        case 4:
            tileset[0].x=c-1;tileset[0].y=0;//|- , T
            tileset[1].x=c-1;tileset[1].y=1;
            tileset[2].x=c-1;tileset[2].y=2;
            tileset[3].x=c;tileset[3].y=1;
            midpointtile=3;
            break;
        case 5:
            tileset[0].x=c;tileset[0].y=0;//_| , L
            tileset[1].x=c;tileset[1].y=1;
            tileset[2].x=c;tileset[2].y=2;
            tileset[3].x=c-1;tileset[3].y=2;
            midpointtile=2;
            break;
        case 6:
            tileset[0].x=c;tileset[0].y=0;//|_ , L
            tileset[1].x=c;tileset[1].y=1;
            tileset[2].x=c;tileset[2].y=2;
            tileset[3].x=c+1;tileset[3].y=2;
            midpointtile=2;
            break;
                        
    }
    b->prepiececolor=color;
}
void ActivePiece :: left(){
    for(int i=0;i<4;i++){
        tileset[i].x--;
    }
}
void ActivePiece :: right(){
    for(int i=0;i<4;i++){
        tileset[i].x++;
    }
}
void ActivePiece :: down(){
    for(int i=0;i<4;i++){
        tileset[i].y++;
    }
}
void ActivePiece :: rotate(int direction){
	Tile temp;
    for(int i=0;i<4;i++){
        temp.x=tileset[i].x;
        temp.y=tileset[i].y;
        tileset[i].x=tileset[midpointtile].x+direction*(tileset[midpointtile].y-temp.y);
        tileset[i].y=tileset[midpointtile].y-direction*(tileset[midpointtile].x-temp.x);
    }
}
bool ActivePiece :: moveDown(){
    Tile ts1[4];
    for(int i=0;i<4;i++){
	ts1[i].x=tileset[i].x;
	ts1[i].y=tileset[i].y+1;
    }
    return b->isFree(ts1);
}
bool ActivePiece :: moveLeft(){
    Tile ts1[4];
    for(int i=0;i<4;i++){
		ts1[i].x=tileset[i].x-1;
		ts1[i].y=tileset[i].y;
    }
    return b->isFree(ts1);
}
bool ActivePiece :: moveRight(){
    Tile ts1[4];
    for(int i=0;i<4;i++){
		ts1[i].x=tileset[i].x+1;
		ts1[i].y=tileset[i].y;
    }
    return b->isFree(ts1);
}
bool ActivePiece :: rotat(int direction){
    Tile ts1[4];
    if(midpointtile==4)return false;
    for(int i=0;i<4;i++){
        ts1[i].x=tileset[midpointtile].x+direction*(tileset[midpointtile].y-tileset[i].y);
        ts1[i].y=tileset[midpointtile].y-direction*(tileset[midpointtile].x-tileset[i].x);
    }
    return b->isFree(ts1);
}
 //----------------------------------------------------------------------------------------------------------------------------------//
Board :: Board(int bx,int by,int bw,int bh): Fl_Widget(bx,by,bw,bh){
    for (int i=0;i<xmaxtiles;i++){
        for(int j=0;j<ymaxtiles;j++){
            occup[i][j]=boardcolor;
        }
    }
    p =new ActivePiece (this);
    nextpiece = new ActivePiece(this);
   score=0;prepiececolor=p->color;level=1;
   
   
 
}
bool Board :: isFree(Tile ts[4]){
	if(inBoard(ts)){
	for(int i=0;i<4;i++){
        if(occup[ts[i].x][ts[i].y]!=boardcolor)return false;
    }
    return true;}
    else return false;
}
bool Board :: inBoard(Tile ts[4]){
    for(int i=0;i<4;i++){
        if(ts[i].x<0||ts[i].y<0||ts[i].x>xmaxtiles-1||ts[i].y>ymaxtiles-1)return false;
	}return true;
}
bool Board :: transferTiles(Tile ts[4],int color){
    for(int i=0;i<4;i++){
        occup[ts[i].x][ts[i].y]=color;
    }
}
void Board :: deleteRow(int row){
    for(int j=row;j>0;j--){
        for(int i=0;i<xmaxtiles;i++){
            occup[i][j]=occup[i][j-1];
        }
    }
    for(int i=0;i<xmaxtiles;i++){
        occup[i][0]=boardcolor;
    }
}
bool Board :: rowDone(int row){
    for(int i=0;i<14;i++){
        if(occup[i][row]==boardcolor)return false;
    }
    return true;
}
bool Board :: gameover(){
	for(int i=0;i<4;i++){
		if(occup[p->tileset[i].x][p->tileset[i].y]!=boardcolor)return true;
	}
	return false;
}		 
int Board :: handle(int e){
	if (e==12||e==8){
		switch(Fl::event_key()){
			case FL_Down : if(p->moveDown())p->down();break;
			case FL_Right : if(p->moveRight())p->right();break;
			case FL_Left : if(p->moveLeft())p->left();break;
			case FL_Up : if(p->rotat(1))p->rotate(1);break;
			case 32 : while(p->moveDown())p->down();break;
		}
	}redraw();
}
void Board :: draw(){
    for(int i=0;i<xmaxtiles;i++){
        for(int j=0;j<ymaxtiles;j++){
            fl_draw_box(FL_BORDER_BOX,i*tilesize,j*tilesize,tilesize,tilesize,occup[i][j]);
        }
    }
    for(int i=0;i<4;i++){
        fl_draw_box(FL_BORDER_BOX,p->tileset[i].x*tilesize,p->tileset[i].y*tilesize,tilesize,tilesize,p->color);
        fl_draw_box(FL_BORDER_BOX,(nextpiece->tileset[i].x+xmaxtiles/2+3)*tilesize,(nextpiece->tileset[i].y+ymaxtiles/2+3)*tilesize,tilesize,tilesize,nextpiece->color);
    }
    fl_color(1);
    fl_line(xmaxtiles*tilesize,0,xmaxtiles*tilesize,ymaxtiles*tilesize);
    fl_line(0,ymaxtiles*tilesize,xmaxtiles*tilesize,ymaxtiles*tilesize);
    
}
void Board::setScoreBox(Fl_Box** sb)
{
	scoreBox = *sb ;
	score=0;
	scoreLabel = (char*) malloc(sizeof(char)*10);
}
int Board :: periodic() {
	redraw();
	if(p->moveDown()){p->down();}
	else {
		transferTiles(p->tileset,p->color);
		for(int i=0;i<4;i++){
			if(rowDone(p->tileset[i].y))
				{deleteRow(p->tileset[i].y);
				score=score+10;}
		}
		p=nextpiece;	
		for(int i=0;i<4;i++){
			fl_draw_box(FL_FLAT_BOX,(nextpiece->tileset[i].x+xmaxtiles/2+3)*tilesize,(nextpiece->tileset[i].y+ymaxtiles/2+3)*tilesize,tilesize,tilesize,bgcolor);
		}		
		nextpiece=new ActivePiece(this);
		level=score/100+1;
		if(gameover()){cout<<"\n\n\n-------------Gameover---------------"<<endl;
			if(score>=100){cout<<"-------------You Played Well-----------"<<endl;
						  cout<<"-------------Level "<<level<<"----------------"<<endl;
						  cout<<"\n--------Your Score is "<<score<<"-------------\n\n\n";exit(0);
			}
			else{cout<<"-------------Level "<<level<<"----------------"<<endl;
				cout<<"\n--------Your Score is "<<score<<"-------------\n\n\n"<<endl;exit(0);			
			}	
		}	
						
	}
	stringstream strs;
	strs << score;
	string temp_str = strs.str();
	strcpy(scoreLabel,"Score: \n");
	strcat(scoreLabel,(char*) temp_str.c_str() );

	scoreBox->label(scoreLabel); 	// void label(char *) itself calls the redraw() for that object.
	//Any other change will require to call scorebox->redraw();
	double time=0.5;
	Fl::repeat_timeout (time/level,timeractions,this);
}
//-----------------------------------------------------------------------------------------------------------------------------------//
void timeractions(void *p) {
	((Board *)p)->periodic ();
}
//-----------------------------------------------------------------------------------------------------------------------------------//
int main(int argc, char *argv[]) {
    	Fl_Window *window = new Fl_Window (600,600,"Tetris"); // outer window
		window->color(bgcolor);   
		Board *b = new Board(0,0,600,600);
		Fl_Box *scorebox = new Fl_Box(tilesize*xmaxtiles+10,50,180,200,"Score: \n 0");
	    scorebox->box(FL_UP_BOX);
        scorebox->labelfont(FL_BOLD+FL_ITALIC);
        scorebox->labelsize(36);
        scorebox->labeltype(FL_ENGRAVED_LABEL);
		b->setScoreBox(&scorebox); // setting scorebox created in main to class. Board
		window->end();  
		window->show();
		Fl::add_timeout(0.0, timeractions,b);
    	return(Fl::run());  // the process waits from here on for events
}



