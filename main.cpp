// I think I want this to be conways game of life.... not sure yet... just learning opengl


#include <iostream>
#include <GL/glut.h>
#include <string>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// prototypes

void init_glSettings( void );
void render_window_callback( void );
void keyboard_input_callback( unsigned char, int, int );
void keyboard_input_special_callback( int, int, int );
void mouse_input_callback( int, int, int, int );
void reshape_callback( int, int );
void draw( void );
void redraw( int );

void draw_rect( float, float, float, float );
void draw_rect_outline( float, float, float, float, float );


// global variables

int window_width = 900;
int window_height = 600;
int window_pos_x = 0;
int window_pos_y = 0;
int grid_num_squares_x = 50;
std::string program_state = "play";

int sim_speed = 50;


int x = 1;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// setup

class Cell{
	public:
	
	int state;
	int num;	
	int r;
	int c;
	int size;		
	float pos_x;
	float pos_y;
	int flag;
	
	void init( int n, int s, float posx, float posy ){
		num = n;
		pos_x = posx;
		pos_y = posy;
		size = s;
		state = 0;
		flag = 0;
	}

	void change_state( void ){
		if( state == 0 ){
			state = 1;
		}else{ state = 0; }
	}

	void set_pos( int x, int y ){
		c = x;
		r = y;
	}

	void flag_state( void ){ flag = 1; }

	void reset_flag( void ){ flag = 0; }
	
	int return_state(){ return state; }

};


Cell *cells = new Cell[ grid_num_squares_x * grid_num_squares_x ];

// will be called before the main loop and after window creation
void init_glSettings( void ){

	glutDisplayFunc( render_window_callback );
	glutKeyboardFunc( keyboard_input_callback );
	glutSpecialFunc( keyboard_input_special_callback );		
	glutReshapeFunc( reshape_callback ); 	
	glutMouseFunc( mouse_input_callback );
}




// this is the function that will render our window
void render_window_callback( void ){	

	if( program_state == "select_gcode" ){			// the main select gcode state
		std::cout << "workging on it"; 

	}else{							// the default state
		draw();	
	}
}




void reshape_callback( int width, int height ){

	std::cout << width << " " << height << "\n" ;
	window_width = width;
	window_height = height;

	glViewport( 0, 0, (GLsizei)width, (GLsizei)height );			// change the size of the view port to the screen width and height
	glMatrixMode( GL_PROJECTION );						// change to the projection matrix
	glLoadIdentity();							// make the projection matrix reset to defaults
	glOrtho( 0.0, (GLdouble)width, 0.0, (GLdouble)height, -1.0, 1.0 );	// set the orthographic projection
										// glOrtho( x min, x max, y min, y max, z min, z max )
	glMatrixMode( GL_MODELVIEW );
	
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// drawing

void draw(){

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glutSwapBuffers();
	
	int row = 0;
	int col = 0;
	int size = window_width / grid_num_squares_x;
	// must initialize the array of cells
	for( int i = 0; i < grid_num_squares_x * grid_num_squares_x; i++ ){
		cells[i].init( i, size, row * size, col * size );
		row += 1;
		
		cells[i].set_pos( row, col );
	
		if( row == grid_num_squares_x ){
			row = 0;
			col += 1;
		}	
	}	

	std::cout << "a\n";
	
	glutTimerFunc(sim_speed, redraw, 0 );
}

void redraw( int t ){

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	int counter = 0;
	int current_cell;
	if( program_state == "play" ){

		for( int i = 0; i < grid_num_squares_x * grid_num_squares_x; i++ ){		
			if( cells[i].flag == 1 ){
				cells[i].change_state();
			}cells[i].reset_flag();
		}
		
		for( int i = 0; i < grid_num_squares_x * grid_num_squares_x; i++ ){
		

			if( cells[i].state == 0 ){
				draw_rect_outline( cells[i].pos_x, cells[i].pos_y, cells[i].size, cells[i].size, 1.0 );			
			}else if( cells[i].state == 1 ){
				draw_rect( cells[i].pos_x, cells[i].pos_y, cells[i].size, cells[i].size );	
			}
			counter = 0;

			if( cells[i].c != 0 && cells[i-1].state == 1){
				counter += 1;
			}
			if( cells[i].c != grid_num_squares_x-1 && cells[i+1].state == 1 ){
				counter += 1;
			}
			if( cells[i].r != 0 && cells[i-grid_num_squares_x].state == 1 ){
				counter += 1;
				//cells[i].change_state();			
			}
			if( cells[i].r != grid_num_squares_x-1 && cells[i+grid_num_squares_x].state == 1 ){
				counter += 1;
				//cells[i].change_state();
			}
			if( cells[i].r != 0 && cells[i].c != 0 && cells[i-grid_num_squares_x-1].state == 1 ){
				counter += 1;
				//cells[i].change_state();			
			}
			if( cells[i].r != 0 && cells[i].c != 0 && cells[i-grid_num_squares_x+1].state == 1 ){
				counter += 1;
				//cells[i].change_state();			
			}
			if( cells[i].r != grid_num_squares_x-1 && cells[i].c != grid_num_squares_x && cells[i+grid_num_squares_x+1].state == 1 ){
				counter += 1;
				//cells[i].change_state();
			}
			if( cells[i].r != grid_num_squares_x-1 && cells[i].c != grid_num_squares_x && cells[i+grid_num_squares_x-1].state == 1 ){
				counter += 1;
				//cells[i].change_state();			
			}		
		
			
			if( counter == 3 && cells[i].state == 0 ){	// if 3 neighbours and current cell is dead
				cells[i].flag_state();
			}else if( counter < 2 && cells[i].state == 1){
				cells[i].flag_state();
			}else if( counter > 3 && cells[i].state == 1){
				cells[i].flag_state();
			}	
		}
	}else{
		for( int i = 0; i < grid_num_squares_x * grid_num_squares_x; i++ ){
			if( cells[i].state == 0 ){
				draw_rect_outline( cells[i].pos_x, cells[i].pos_y, cells[i].size, cells[i].size, 1.0 );			
			}else if( cells[i].state == 1 ){
				draw_rect( cells[i].pos_x, cells[i].pos_y, cells[i].size, cells[i].size );	
			}
		}
	}	
	
	glutSwapBuffers();
	
	glutTimerFunc(sim_speed, redraw, 0 );
}




void draw_rect( float x, float y, float width, float height ){
	glBegin( GL_POLYGON );
		glVertex3f( x, y, 0.0 );
		glVertex3f( x, y+height, 0.0 );
		glVertex3f( x+width, y+height, 0.0 );
		glVertex3f( x+width, y, 0.0 );
	glEnd();	
}


void draw_rect_outline( float x, float y, float width, float height, float thickness ){
	glBegin( GL_POLYGON );
		draw_rect( x,y,width, thickness );
		draw_rect( x,y,thickness, height );		
		draw_rect( x+width,y,thickness, height );
		draw_rect( x,y+width,width, thickness );		
	glEnd();	
}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//input

// will handle ansii keyboard input
void keyboard_input_callback( unsigned char key, int x, int y ){

	switch( key ){
		case 'z' : 	{
				std::cout << "pressed z.... pausing/unpausing \n";
				if( program_state == "play"){ program_state = "pause"; }else{ program_state = "play"; }
				std::cout << program_state << " \n";
				break;
				}
	}
}

// will andle other keyboard input
void keyboard_input_special_callback( int key, int x, int y ){

	std::cout << key << "\n";
}



void mouse_input_callback( int button, int state, int x, int y ){
	std::cout << button <<	" " << state << " " << x << " " << y << " \n";
	if( button == 0 && state == 0 ){	
		for( int i = 0; i < grid_num_squares_x * grid_num_squares_x; i++  ){
			if( x > cells[i].pos_x && x < cells[i].pos_x + cells[i].size && (window_height - y) > cells[i].pos_y && (window_height - y) < cells[i].pos_y + cells[i].size ){
				cells[i].change_state();
			}
		}
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main

int main( int argc, char * * argv ){
	
	glutInit( &argc, argv );
	glutInitWindowSize( window_width, window_height );
	glutInitWindowPosition( 0, 0 );		
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE );	
	glutCreateWindow( "hello world" );

	init_glSettings();
	
	glutMainLoop();

	return 0;
}
