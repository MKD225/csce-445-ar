/*#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glut.h>
#else
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif*/

#include <stdio.h>
#include <windows.h>
#include <GL/glut.h>

#include <AR/gsub.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>


#include "tutorial4.h"
#include "texture.h"
#include "3dsloader.h"
#include "gui.h"
//
// Camera configuration.
//
#ifdef _WIN32
char			*vconf = "Data\\WDM_camera_flipV.xml";
#else
char			*vconf = "";
#endif

int             xsize, ysize;
int             thresh = 100;
int             count = 0;

char           *cparam_name    = "Data/camera_para.dat";
ARParam         cparam;

char           *patt_name      = "Data/patt.hiro";
int             patt_id;
double          patt_width     = 80.0;
double          patt_center[2] = {0.0, 0.0};
double          patt_trans[3][4];

obj_type object;

typedef int bool;
#define false 0
#define true 1

bool can_draw_model = false;

static void   init(void);
static void   cleanup(void);
static void   keyEvent( unsigned char key, int x, int y);
static void   mainLoop(void);
static void   draw( void );



int main(int argc, char **argv)
{
	glutInit(&argc, argv);



	init();

	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowSize(winw,winh);
	glutInitWindowPosition(200,100);

	
	glShadeModel(GL_SMOOTH); // Type of shading for the polygons
	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
    


	//glutDisplayFunc(draw);
	glutReshapeFunc(Resize);
	//glutMouseFunc();
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);

    arVideoCapStart();


	glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping
    //Load3DS (&object,"sofa.3DS");


	//*******************************************************************************************
	//load model here
	//*******************************************************************************************


	Load3DS (&object,"spaceship.3ds");

	object.id_texture=LoadBitmap2("spaceshiptexture.bmp"); // The Function LoadBitmap() return the current texture ID
    
	printf("texture id: %d\n",object.id_texture);

    // If the last function returns -1 it means the file was not found so we exit from the program
    if (object.id_texture==-1)
    {
        MessageBox(NULL,"Image file: spaceshiptexture.bmp not found", "Zetadeck",MB_OK | MB_ICONERROR);
        exit (0);
    }


    argMainLoop( MouseButton, keyEvent, mainLoop );


	return (0);
}

static void   keyEvent( unsigned char key, int x, int y)
{
    /* quit if the ESC key is pressed */
    if( key == 0x1b ) {
        printf("*** %f (frame/sec)\n", (double)count/arUtilTimer());
        cleanup();
        exit(0);
    }

	else if (key == 'w') {
		deltaZ += 0.4;
	}

	else if (key == 's') {
		deltaZ -= 0.4;
	}

		else if (key == 'a') {
		deltaY += 0.4;
	}

	else if (key == 'd') {
		deltaY -= 0.4;
	}

	else if (key == 'y') {
		TheButtonCallback();
	}
}

/* main loop */
static void mainLoop(void)
{
    ARUint8         *dataPtr;
    ARMarkerInfo    *marker_info;
    int             marker_num;
    int             j, k;

    /* grab a vide frame */
    if( (dataPtr = (ARUint8 *)arVideoGetImage()) == NULL ) {
        arUtilSleep(2);
        return;
    }
    if( count == 0 ) arUtilTimerReset();
    count++;

    argDrawMode2D();
    argDispImage( dataPtr, 0,0 );

    /* detect the markers in the video frame */
    if( arDetectMarker(dataPtr, thresh, &marker_info, &marker_num) < 0 ) {
        cleanup();
        exit(0);
    }

    arVideoCapNext();

    /* check for object visibility */
    k = -1;
    for( j = 0; j < marker_num; j++ ) {
        if( patt_id == marker_info[j].id ) {
            if( k == -1 ) k = j;
            else if( marker_info[k].cf < marker_info[j].cf ) k = j;
        }
    }
    if( k == -1 ) {
        //argSwapBuffers();
        //return;
		can_draw_model = false;
    }

	else {
		can_draw_model = true;
	}

    /* get the transformation between the marker and the real camera */
    arGetTransMat(&marker_info[k], patt_center, patt_width, patt_trans);

    draw();

    argSwapBuffers();
}

static void init( void )
{
    ARParam  wparam;
	
    /* open the video path */
    if( arVideoOpen( vconf ) < 0 ) exit(0);
    /* find the size of the window */
    if( arVideoInqSize(&xsize, &ysize) < 0 ) exit(0);
    printf("Image size (x,y) = (%d,%d)\n", xsize, ysize);

    /* set the initial camera parameters */
    if( arParamLoad(cparam_name, 1, &wparam) < 0 ) {
        printf("Camera parameter load error !!\n");
        exit(0);
    }
    arParamChangeSize( &wparam, xsize, ysize, &cparam );
    arInitCparam( &cparam );
    printf("*** Camera Parameter ***\n");
    arParamDisp( &cparam );

    if( (patt_id=arLoadPatt(patt_name)) < 0 ) {
        printf("pattern load error !!\n");
        exit(0);
    }

    /* open the graphics window */
    argInit( &cparam, 1.0, 0, 0, 0, 0 );
}

/* cleanup function called when program exits */
static void cleanup(void)
{
    arVideoCapStop();
    arVideoClose();
    argCleanup();
}


static void drawModel() {

	int l_index;
	//glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping
	glEnable(GL_COLOR_MATERIAL);
	glBindTexture(GL_TEXTURE_2D, object.id_texture); // We set the active texture 

    glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
    for (l_index=0;l_index<object.polygons_qty;l_index++)
    {
        //----------------- FIRST VERTEX -----------------
        // Texture coordinates of the first vertex
        glTexCoord2f( object.mapcoord[ object.polygon[l_index].a ].u,
                      object.mapcoord[ object.polygon[l_index].a ].v);
        // Coordinates of the first vertex
        glVertex3f( object.vertex[ object.polygon[l_index].a ].x,
                    object.vertex[ object.polygon[l_index].a ].y,
                    object.vertex[ object.polygon[l_index].a ].z); //Vertex definition

        //----------------- SECOND VERTEX -----------------
        // Texture coordinates of the second vertex
        glTexCoord2f( object.mapcoord[ object.polygon[l_index].b ].u,
                      object.mapcoord[ object.polygon[l_index].b ].v);
        // Coordinates of the second vertex
        glVertex3f( object.vertex[ object.polygon[l_index].b ].x,
                    object.vertex[ object.polygon[l_index].b ].y,
                    object.vertex[ object.polygon[l_index].b ].z);
        
        //----------------- THIRD VERTEX -----------------
        // Texture coordinates of the third vertex
        glTexCoord2f( object.mapcoord[ object.polygon[l_index].c ].u,
                      object.mapcoord[ object.polygon[l_index].c ].v);
        // Coordinates of the Third vertex
        glVertex3f( object.vertex[ object.polygon[l_index].c ].x,
                    object.vertex[ object.polygon[l_index].c ].y,
                    object.vertex[ object.polygon[l_index].c ].z);
    }

    glEnd();
	glFlush(); // This force the execution of OpenGL commands
	glDisable(GL_TEXTURE_2D);
}

static void draw( void )
{
    double    gl_para[16];
    GLfloat   mat_ambient[]     = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash[]       = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {600.0,400.0,800.0,0.0};
    /*GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};
    */

    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glClearDepth( 1.0 );
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    /* load the camera transformation matrix */
    argConvGlpara(patt_trans, gl_para);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );

	
    glEnable(GL_LIGHTING);
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
	glEnable(GL_LIGHT0);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
    //glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambi);
    glMatrixMode(GL_MODELVIEW);
    glTranslatef( 0.0, 0.0, 25.0 );
	glTranslatef(updateX, updateY, updateZ);

	/************************************************************************/
	/*Model Goes Here*/
	/************************************************************************/

	if(can_draw_model == true) {
		drawModel();
	}

	

    glDisable( GL_LIGHTING );

    glDisable( GL_DEPTH_TEST );


	/*
	 *	Disable depth test and lighting for 2D elements
	 */
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

/*
	 *	Set the orthographic viewing transformation
	 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,winw,winh,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*
	 *	Draw the 2D overlay
	 */
	Draw2D();

	/*
	 *	Bring the back buffer to the front and vice-versa.
	 */
	//glutSwapBuffers();
}


