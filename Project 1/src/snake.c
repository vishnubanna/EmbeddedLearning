#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h> // for random()
#include "screen.h"
#include "tty.h"
#include "snake.h"
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

// The characters to be shown and their colors.
//char scr[80][24];
//char color[80][24];

char scr[80][24];
char color[80][24];
char players[12][18]={0};
char colors[6] = {0xa0, 0xe0, 0x90, 0xb0, 0xc0, 0xd0};

// Data variables for the game...
int px;
int py;
int dx;
int dy;
int prevdx;
int prevdy;
int money;
enum { SPLASH, RUNNING } phase = SPLASH;
int splash_ticks;
char health = 100;
char healthd = 0;
char chr = 0xa0;
char cindx = 0;
char playerl = 2;
char portalx = 0;
char portaly = 0;
char ptx = 0;
char pty = 0;

#define MAXLEN 40
struct {
  int8_t x;
  int8_t y;
} body[MAXLEN];
int bodylen;

// Print a message at screen coordinate x,y in color c.
void msg(int x, int y, int c, const char *s)
{
  int n;
  int len = strlen(s);
  for(n=0; n<len && n+x<80; n++) {
    color[x+n][y] = c;
    scr[x+n][y] = s[n];
  }
}

// Check two points, (x1,y1) and (x2,y2) and return a 1
// if they are within 5 character cells of each other.
int tooclose(int x1, int y1, int x2, int y2)
{
  int x = x1 - x2;
  int y = y1 - y2;
  if (x*x + y*y < 25)
    return 1;
  return 0;
}

// Put a dollar sign in a random location of the screen.
// Make sure that there is nothing there though.
// Also, make sure it is not too close to the snake's head.
// Let's not make the game too easy.
void newmoney(void)
{
  int x,y;
  do {
    x = (random() % 60) + 1;
    y = (random() % 22) + 1;
  } while(scr[x][y] != ' ' || tooclose(x,y, px,py));
  scr[x][y] = '$';
  color[x][y] = 0xe0;
}

void newportal(void)
{
  int x,y;
  do {
    x = (random() % 50) + 5;
    y = (random() % 12) + 6;
  } while(scr[x][y] != ' ' || tooclose(x,y, px,py));
  scr[x][y] = '\\';
  color[x][y] = 0x90;
  ptx = x;
  pty = y;

  do {
    x = (random() % 50) + 5;
    y = (random() % 12) + 6;
  } while(scr[x][y] != ' ' || tooclose(x,y, ptx,pty));
  portalx = x;
  portaly = y;

  scr[x][y] = '\\';
  color[x][y] = 0xa0;
}

// Eat some money.  Increment the money counter.
// Display the total money in the upper right of the screen.
void getmoney(void)
{
  scr[ptx][pty] = ' ';
  scr[portalx][portaly] = ' ';
  money++;
  newmoney();
  newportal();
}


static char gtc(void)
{
    TIM2->CR1 &= ~TIM_CR1_CEN;
    //while(!available());
    //getchar();
    char ind = 9;
    msg(25,3, 0x0f, "PLEASE ENTER YOUR INITIALS");
    render();
    char letter = 0;
    char buff[18];
    char i;
    for (i = 0; i < 6; i++){
  	  while(!available());
  	  letter = getchar();
  	  if (letter != '\n'){
  		  if (letter == 8){
  			  i-=1;
			  if (i >= 0){
				 players[playerl][i] = ' ';
				 i-=1;
			  }
			  else{
				  i = -1;
			  }
  		  }
  		  else{
  			 players[playerl][i] = letter;
  		  }
  	  }
  	  else if (letter == '\n'){
  		  break;
  	  }
      sprintf(buff, "name: %s", players[playerl]);
	  msg(62,3, 0x0f, buff);
	  render();
    }
    sprintf(buff, "%s", players[playerl]);
    sprintf(players[playerl], "%6s|%2d|%2d", buff, money, bodylen);
    msg(62,3, 0x00, "                  ");
    TIM2->CR1 |= TIM_CR1_CEN;
}

static void rtname(char update){
	sprintf(players[0], "player:  M L");
	msg(62,12,0x90, players[0]);
	for (char x = 1; x < 11; x++){
	  //players[x] = "        ";
	  msg(62,x + 12,0x90, players[x]);
	}
	if (update)
		playerl = (playerl)%11 + 1;
}

// Indicate a collision with a big red X.
// Also display a "Game over" statement.
void collision(void)
{
  msg(px,py,0x90, "X");
  msg(px-1,py-1,0x90, "\\");
  msg(px+1,py-1,0x90, "/");
  msg(px+1,py+1,0x90, "\\");
  msg(px-1,py+1,0x90, "/");

  msg(62,3, 0x0f, "name: ");
  gtc();

  if (py > 12) {
    msg(30,8, 0x0f, "   Game over   ");
    msg(30,9, 0x0f, " Press any key ");
    //msg(30,10, 0x0f, " enter initals ");
  } else {
    msg(30,15, 0x0f,"   Game over   ");
    msg(30,16, 0x0f," Press any key ");
  }

//  char buf[10];
//  sprintf(buf, "health:%d", health);
//  msg(62,8, 0xe0, buf); // x,y,color,string
  //TIM2->CR1 &= ~TIM_CR1_CEN;
  rtname(1);
  //TIM2->CR1 |= ~TIM_CR1_CEN;
}

// Initialize the game data structures.
void init(void)
{
  cindx = 0;
  chr = colors[cindx];
  int x,y;
  for(y=0; y<24; y++)
    for(x=0; x<80; x++) {
      scr[x][y] = ' ';
      color[x][y] = 0xf0;
    }

  for(x=0; x<80; x++) {
    if (x==0 || x==61 || x==79) {
      msg(x,0,0xb0,"+");
      msg(x,23,0xb0,"+");
    } else {
      msg(x,0,0xb0,"-");
      msg(x,23,0xb0,"-");
    }
  }
  for(y=1; y<23; y++) {
    msg(0,y, 0xb0, "|");
    msg(61,y, 0xb0, "|");
    msg(79,y, 0xb0, "|");
  }

  rtname(0);

  px=40;
  py=12;
  scr[px][py] = '@';
  color[px][py] = 0xa0;
  bodylen = 6;
  body[0].x = px;
  body[0].y = py;
  for(x=1; x<6; x++) {
    scr[px-x][py] = '-';
    color[px-x][py] = 0xa0;
    body[x].x = px-x;
    body[x].y = py;
  }
  dx=1;
  dy=0;
  prevdx = dx;
  prevdy = dy;

  money = 0;
  newmoney();
}

// Dump the scr and color arrays to the terminal screen.
void render(void)
{
  int x,y;
  home();
  int col = color[0][0];
  fgbg(col);
  for(y=0; y<24; y++) {
    setpos(0,y);
    for(x=0; x<80; x++) {
      if (color[x][y] != col) {
        col = color[x][y];
        fgbg(col);
      }
      putchar(scr[x][y]);
    }
  }
  fflush(stdout);
}

// Display the initial splash screen.
void splash(void)
{
  clear();
  int x,y;
  for(y=0; y<24; y++)
    for(x=0; x<80; x++) {
      scr[x][y] = ' ';
      color[x][y] = 0x70;
    }
  msg(30,8, 0x0a, "                 ");
  msg(30,9, 0x0a, "  Hungry Snake   ");
  msg(30,10,0x0a, "  Press Any Key  ");
  msg(30,11,0x0a, "                 ");
  render();
}

// Extend the snake into the new position.
void extend(void)
{
  scr[px][py] = '@'; // draw new head
  color[px][py] = 0xa0;
  if (dx != 0) {
    if (prevdx != 0) // Did we turn a corner?
      scr[body[0].x][body[0].y] = '-'; // no
    else
      scr[body[0].x][body[0].y] = '+'; // yes
  } else {
    if (prevdy != 0) // Did we turn a corner?
      scr[body[0].x][body[0].y] = '|'; // no
    else
      scr[body[0].x][body[0].y] = '+'; // yes
  }
  int n;
  for(n=bodylen; n>=0; n--)
    body[n] = body[n-1];
  body[0].x = px;
  body[0].y = py;
  bodylen += 1;

}

// Move the snake into the new position.
// And erase the last character of the tail.
void move(void)
{
	cindx = (cindx + 1)%6;
	chr = colors[cindx];
	if (health < 50){
		chr = 0xe0;
	}

    scr[body[bodylen-1].x][body[bodylen-1].y] = ' '; // erase end of tail
    scr[px][py] = '@'; // draw new head
    color[px][py] = chr;
    if (dx != 0) {
      if (prevdx != 0){ // Did we turn a corner?
        scr[body[0].x][body[0].y] = '-'; // no
      }
      else{
        scr[body[0].x][body[0].y] = '+'; // yes
      	health -= healthd;
      }
    } else {
      if (prevdy != 0){ // Did we turn a corner?
        scr[body[0].x][body[0].y] = '|'; // no
      }
      else{
        scr[body[0].x][body[0].y] = '+'; // yes
      	health -= healthd;
      }
    }
    int n;
    for(n=bodylen-1; n>0; n--) {
      body[n] = body[n-1];
    }
    body[0].x = px;
    body[0].y = py;

}

// Interpret a key press and update the data structures.
void update(char in)
{
  prevdx = dx;
  prevdy = dy;
  switch(in) {
    case 'a':
    case 'h': dx=-1; dy=0; break;
    case 's':
    case 'j': dx=0; dy=1; break;
    case 'w':
    case 'k': dx=0; dy=-1; break;
    case 'd':
    case 'l': dx=1; dy=0; break;
    default: break;
  }

  px += dx;
  py += dy;

  //if the head is at a dollar
  if (scr[px][py] == '$') {
	healthd = 0;
    getmoney();
    if (bodylen == MAXLEN-1)
      msg(26,0,0x90,"Omega-Snake-Status-Achieved");
    if (bodylen < MAXLEN)
      extend();
    else
      move();
  //its not at a dollar

   }else if (scr[px][py] == '\\'){
	   scr[ptx][pty] = ' ';
	   scr[portalx][portaly] = ' ';
	   if (px == portalx){
	   	   px = ptx;
	   	   py = pty;
	   }else{
	   	   px = portalx;
	   	   py = portaly;
	   }
	   dx = -dx;
	   dy = -dy;
       move();
   }else if (scr[px][py] == ' ') {
	healthd = 1;
    char buf[30] = {0};

    if (health == 99){
    	sprintf(buf, "Health:      ", health);
    	msg(63,8, 0xe0, buf); // x,y,color,string
    }
    //char buf[10];
    sprintf(buf, "Health:%d", health);
    msg(63,8, 0xe0, buf); // x,y,color,string

    sprintf(buf, "Length:%d", bodylen);
    msg(63,7,0xe0,buf);

    sprintf(buf, "Money:%d", money);
    msg(63,6, 0xe0, buf);

	if (health == 0){
		collision();
		phase = SPLASH;
		splash_ticks=0;
		health = 100;
		chr = 0xa0;
	}
	else{
		move();
	}

  } else {
    collision();
    phase = SPLASH;
    splash_ticks=0;
	health = 100;
	chr = 0xa0;
  }
}

void animate(void)
{
  if (phase == SPLASH) {
    if (splash_ticks < 10) {
      while (available())
        getchar();
      splash_ticks++;
      return;
    }
    // Stall waiting for a key.
    while (!available())
      ;
    getchar();
    // Get the timer counter value for the random seed.
    int seed=get_seed();
    srandom(seed);
    health = 100;
    init();
    clear();
    phase = RUNNING;
  }
  char in=' ';
  if (phase == RUNNING) {
    while(available()) {
      in = getchar();
    }
    if (in == 'q') {
#ifdef __linux__
      cursor_on();
      cooked_mode();
      exit(0);
#else
      splash();
      render();
      phase = SPLASH;
      splash_ticks = 0;
#endif
    }
    if (in == 'p') {
      freeze();
    }
    update(in);
    render();
    return;
  }
}
