#include <stdint.h>
#include <TouchScreen.h>
#include <TFT.h>

#include <math.h>

int RESX = 240;
int RESY = 320;

int HALFX=RESX/2;
int HALFY=RESY/2; 

float LAST_RX = 0.0;
float LAST_RY = 0.0;
float LAST_RZ = 0.0;
int   LASTCOLOR = 0;
  
 static unsigned int TS_MINX, TS_MAXX, TS_MINY, TS_MAXY;
 static unsigned int MapX1, MapX2, MapY1, MapY2;
 TouchScreen ts = TouchScreen(17, A2, A1, 14, 300);
 
 /****************/
 
#define OFFSETX 120                  // offset for screen wont change unless
#define OFFSETY 175                 // i use different screen! so its kinda fixed
#define OFFSETZ 30


/////////////////


const int spx = 10;
const int aa[9]={  spx ,-spx ,-spx,spx   ,  spx,-spx,-spx ,spx   };  // x data for shape vertex
const int bb[9]={  spx ,spx  ,-spx,-spx  ,  spx,spx ,-spx ,-spx  };  // y data for shape vertex
const int cc[9]={ -spx ,-spx ,-spx,-spx  ,  spx,spx ,spx  ,spx   };  // z data for shape vertex

const int ff[12]={1,2,3,4,  5,6,7,8, 1,2,3,4};            // start vertex for lines
const int gg[12]={2,3,4,1,  6,7,8,5, 5,6,7,8};            // end vertex for lines





void bresenham(int x0, int x1, int y0, int y1,uint16_t color){

    int dx = abs(x1-x0) , sx = x0<x1 ? 1 : -1;
    int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = dx+dy, e2; 
    for (;;){ 
        Tft.setPixel(x0,y0,color);
        e2 = 2*err;
        if (e2 >= dy) { /* e_xy+e_x > 0 */
            if (x0 == x1) break;
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) { /* e_xy+e_y < 0 */
            if (y0 == y1) break;
            err += dx;
            y0 += sy;
        }
    }
}



void cube(float rotx,float roty,float rotz,int passcolor)       
 {
 int newx[8];                       // translated screen x co-ordinates for vertex
 int newy[8];                       // translated screen y co-ordinates for vertex
 int i,ii,loopi;                      // temp variable for loops
 float xt,yt,zt,x,y,z,sinax,cosax,sinay,cosay,sinaz,cosaz;  // lots of work variables
 
 int sx,sy,ex,ey;                    
 int vertex;
 
 float xpos=0;                // position for object in 3d space, in x
 float ypos=0;                // y
 float zpos=0;                // and z values
 //float rotx=0;                      // starting amount of x rotation
 //float roty=0;                         // starting amount of y rotation
 //float rotz=0;                        // starting amount of z rotation

 
 int numdraw = 1;
 
  xpos=xpos+0.0;           
  ypos=ypos+0.0;           
  zpos=zpos+0.0;           

  rotx=rotx+.1;             
  roty=roty+.1;             
  rotz=rotz+0;             


  sinax=sin(rotx);            // precalculate the sin and cos values
  cosax=cos(rotx);            // for the rotation as this saves a
 
  sinay=sin(roty);            // little time when running as we
  cosay=cos(roty);            // call sin and cos less often
 
  sinaz=sin(rotz);            // they are slow routines
  cosaz=cos(rotz);            // and we dont want slow!

  for (i=0; i<8; i++)               // translate 3d vertex position to 2d screen position
        {
        x=aa[i];                    
        y=bb[i];                    
        z=cc[i];                   

        yt = y * cosax - z * sinax;    // rotate around the x axis
        zt = y * sinax + z * cosax;    // using the Y and Z for the rotation
        y = yt;
        z = zt;

        xt = x * cosay - z * sinay;    // rotate around the Y axis
        zt = x * sinay + z * cosay;    // using X and Z
        x = xt;
        z = zt;

        xt = x * cosaz - y * sinaz;    // finaly rotate around the Z axis
        yt = x * sinaz + y * cosaz;    // using X and Y
        x = xt;
        y = yt;

        x=x+xpos;            // add the object position offset
        y=y+ypos;            // for both x and y
        z=z+OFFSETZ-zpos;        // as well as Z

        newx[i]=(x*64/z)+OFFSETX;    // translate 3d to 2d coordinates for screen
        newy[i]=(y*64/z)+OFFSETY;    // drawing so we can see the cube

        }

           
    // delay for a while to allow looking at the cube

  for (ii=0; ii<12; ii++)        // draw the lines that make up the object
        {
          vertex=ff[ii]-1;         // -1 temp = start vertex for this line
          sx=newx[vertex];        // set line start x to vertex[i] x position
          sy=newy[vertex];        // set line start y to vertex[i] y position
         
         // Serial.write(vertex);
         
          vertex=gg[ii]-1;         // -1 temp = end vertex for this line
          ex=newx[vertex];    // set line end x to vertex[i+1] x position
          ey=newy[vertex];    // set line end y to vertex[i+1] y position
          // Serial.write(vertex);
         // delay(200);
          //Tft.drawLine(sx, sy, ex, ey ,WHITE);// draw the line between these 2 vertex
          int sz = 3;
          
          if (passcolor==0){
           //Tft.setPixel(sx,sy,WHITE); 
           //Tft.drawLine(sx, sy, ex, ey ,WHITE);// draw the line between these 2 vertex
           bresenham(sx, ex, sy, ey,0xffff); 
           //Tft.setPixel(ex,ey,WHITE); 
            
          }
          if (passcolor==1){
            bresenham(sx, ex, sy, ey,0xffff); 
            //Tft.setPixel(ex,ey,WHITE); 
          }    
            if (passcolor==2){      
               bresenham(sx, ex, sy, ey,0x0000); 
               //Tft.setPixel(ex,ey,WHITE);   
            }      
        }

 
  
 }//cube function
  

  
/************************/

void setup(void)
{
 
  //Serial.begin(9600);
 
 
  Tft.init();//init TFT
  initTouchScreenParameters(); //initializes Touch Screen parameters based on the detected TFT Touch Schield hardware


}
 

 
void loop(void)
{

  
  
  
  // a point object holds x y and z coordinates
  Point p = ts.getPoint();
 
  p.x = map(p.x, TS_MINX, TS_MAXX, MapX1, MapX2);
  p.y = map(p.y, TS_MINY, TS_MAXY, MapY1, MapY2);
 
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > ts.pressureThreshhold)
  {
    //Tft.drawRectangle(p.x,p.y,10,10,WHITE);
    //if (LAST_RY>=360) {LAST_RY =0;LAST_RX=10;  };
    // if (LAST_RY<=0)   {LAST_RY =0;LAST_RX=-10; };
       
    if (p.x >HALFX){
      

       //Serial.write("move right!");
       LAST_RY = LAST_RY+.1;
       LASTCOLOR =0;
       
       LAST_RX = LAST_RX+.01;
    }
  
    if (p.x <=HALFX){
       //Serial.write("move left!");
       
        LAST_RY = LAST_RY-.1;   
        LASTCOLOR =1;    
        
        LAST_RX = LAST_RX+.1;
    }

     cube(LAST_RX,LAST_RY,LAST_RZ,LASTCOLOR);
     //delay(10);
     cube(LAST_RX,LAST_RY,LAST_RZ,2); 
 
  }else{
     cube(LAST_RX,LAST_RY,LAST_RZ,LASTCOLOR);
     //delay(10);
     cube(LAST_RX,LAST_RY,LAST_RZ,2);   

  }
  
  
}




/*****************************/

void initTouchScreenParameters()
{
  //This function initializes Touch Screen parameters based on the detected TFT Touch Schield hardware
 
  if(Tft.IC_CODE == 0x5408) //SPFD5408A TFT driver based Touchscreen hardware detected
  {
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ts = TouchScreen(54, A1, A2, 57, 300); //init TouchScreen port pins
#else
    ts = TouchScreen(14, A1, A2, 17, 300); //init TouchScreen port pins
#endif
    //Touchscreen parameters for this hardware
    TS_MINX = 120;
    TS_MAXX = 910;
    TS_MINY = 120;
    TS_MAXY = 950;
 
    MapX1 = 239;
    MapX2 = 0;
    MapY1 = 0;
    MapY2 = 319;
  }
  else //ST7781R TFT driver based Touchscreen hardware detected
  {
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ts = TouchScreen(57, A2, A1, 54, 300); //init TouchScreen port pins
#else
    ts = TouchScreen(17, A2, A1, 14, 300); //init TouchScreen port pins
#endif
 
    //Touchscreen parameters for this hardware
    TS_MINX = 140;
    TS_MAXX = 900;
    TS_MINY = 120;
    TS_MAXY = 940;
 
    MapX1 = 239;
    MapX2 = 0;
    MapY1 = 319;
    MapY2 = 0;
  }
}

