#include <cstdio>
#include <cstdlib>
/****************************************************************/
/* works on mac using freeglut and xquartz                      */
/* you can install freeglut using homebrew                      */
/* xquartz can be downloaded at http://xquartz.macosforge.org   */
/****************************************************************/
#include <GL/freeglut.h>
#include <jpeglib.h>
#include <jerror.h>
#include<math.h>
#ifdef __WIN32
#pragma comment (lib, "jpeg.lib")
#endif

unsigned char image[1920*1280*3];
char presse;
float angleRotate;//angle de rotation
float zoom=12;
int anglex=40,angley=30,x,y,xold,yold;
void affichage();
void clavier(unsigned char touche,int x,int y);
void keyboard(int touche,int x,int y);
void souris(int boutton, int etat,int x,int y);
void sourismouv(int x,int y);
void redim(int l,int h);
void loadJpegImage(char *fichier);
void animate();
int main(int argc,char **argv)

{
  /* Chargement de la texture */
  loadJpegImage("./text.jpg");

  /* Creation de la fenetre OpenGL */
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowPosition(200,200);
  glutInitWindowSize(500,500);
  glutCreateWindow("Texture JPEG");

  /* Initialisation de l'etat d'OpenGL */
  glClearColor(0.0,0.0,0.0,0.0);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);


  /* Mise en place de la projection perspective */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0,1,1.0,5.0);
  glMatrixMode(GL_MODELVIEW);

  /* Parametrage du placage de textures */
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,1920,1280,0,GL_RGB,GL_UNSIGNED_BYTE,image);
  glEnable(GL_TEXTURE_2D);

  /* Mise en place des fonctions de rappel */
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutMouseFunc(souris);
  glutMotionFunc(sourismouv);
  glutSpecialFunc(keyboard);
  glutReshapeFunc(redim);
  glutIdleFunc(animate);

  /* Entrée dans la boucle principale glut */
  glutMainLoop();
  return 0;
}
// la fonction qui permet d'activer la lumiere
void lumiereopen(){
//sources des lumieres
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glEnable(GL_COLOR_MATERIAL);

//position de la lumiere
   GLfloat position[]={0,2,0,1};
   GLfloat direction[] ={1,0,1};
   GLfloat position2[] ={0,1,1,1};
   GLfloat intensite[] ={1.5,1.5,1.5};

   //

   glLightfv(GL_LIGHT0,GL_POSITION,position);
    glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,direction);
    glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,0.5);
    glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,-120);

    glLightfv(GL_LIGHT0,GL_POSITION,position2);
    glLightfv(GL_LIGHT0,GL_AMBIENT,intensite);

}
// pour eteindre la lumiere
void lumierclose(){
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHT1);
}

GLfloat angle = 0;
GLfloat angle1 = 0;

// une methode qui gére l'animaton via le clavier b
void animate2()
{
    angle += 1.0;
    glutPostRedisplay();
}
//une method qui gére l'animation via le clavier a
void animate1()
{
    angle1 += 1.0;
    glutPostRedisplay();
}
//animation automatique
void animate(){
    angleRotate+=0.5;
    glutPostRedisplay();
};




void affichage()
{

 glShadeModel(GL_SMOOTH);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //glOrtho(-10,10,-10,10,-5,5);
  glOrtho(zoom,-zoom,-zoom,zoom,zoom,-zoom);
  glMatrixMode(GL_MODELVIEW);

  /* effacement de l'image avec la couleur de fond */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//glShadeModel(GL_SMOOTH);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
//utilisation des lumieres



  gluLookAt(0.0,0.0,2.5,0.0,0.0,0.0,0.0,1.0,0.0);
  glRotatef(angley,1.0,0.0,0.0);
  glRotatef(anglex,0.0,1.0,0.0);
  glColor3f(1.0,1.0,1.0);
glFlush();

//modélisation ddu tortue
glPushMatrix();
//animation automatique
glRotated(angleRotate,0.0,-0.5,0.1);
//animation au clavier
glTranslated(angle,1.0,0.0);
glTranslated(-angle1,-1.0,0.0);
  //modélisation du carapace via des paramétres
    glPushMatrix();
    int i, r=3;
    float tab[12][2];
    for (i=0;i<12;i++){
        tab[i][0]=cos((i*M_PI)/4);
        tab[i][1]=sin((i*M_PI)/4);
    }
    glBegin(GL_POLYGON);
    for(i=0;i<12;i++){
        glTexCoord2f(2.0+tab[i][0],2.0+tab[i][1]);
        glVertex3f(r*tab[i][0],0.0,r*tab[i][1]);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for(i=0;i<12;i++){
        glTexCoord2f(2.0+tab[i][0],2.0+tab[i][1]);
        glVertex3f((r/2.0)*tab[i][0],2.0,(r/2)*tab[i][1]);
    }
    glEnd();


    for(i=0;i<12;i++){
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0,0.0); glVertex3f((r/2.0)*tab[i][0],2.0,(r/2.0)*tab[i][1]);
        glTexCoord2f(0.0,1.0); glVertex3f(r*tab[i][0],0.0,r*tab[i][1]);
        glTexCoord2f(1.0,1.0); glVertex3f(r*tab[(i+1)%12][0],0.0,r*tab[(i+1)%12][1]);
        glTexCoord2f(1.0,0.0); glVertex3f((r/2.0)*tab[(i+1)%12][0],2.0,(r/2.0)*tab[(i+1)%12][1]);


    glEnd();
    }


    glPopMatrix();
// Modélisation des pattes
//1ére patte
    glPushMatrix();
    glColor3f(0.8,0.6,0.4);
    glTranslatef(2.0,-0.35,1.5);
    glutSolidCube(0.7);
    glPopMatrix();
//2éme patte
    glPushMatrix();
    glColor3f(0.8,0.6,0.4);
    glTranslatef(-2.0,-0.35,1.5);
    glutSolidCube(0.7);
    glPopMatrix();

// 3éme patte

    glPushMatrix();
    glColor3f(0.8,0.6,0.4);
    glTranslatef(-2.0,-0.35,-1.5);
    glutSolidCube(0.7);
    glPopMatrix();
//4éme patte

    glPushMatrix();
    glColor3f(0.8,0.6,0.4);
    glTranslatef(2.0,-0.35,-1.5);
    glutSolidCube(0.7);
    glPopMatrix();


// queue du tortue

    glPushMatrix();
    glTranslated(0.0,0.47,2.0);
    glutSolidCone(0.5,2.0,10,10);
    glPopMatrix();


//cou
    glPushMatrix();
    glTranslated(0.0,0.4,-3.4);
    glutSolidCylinder(0.4,1.5,10.0,10.0);
    glPopMatrix();

// sphere

    glPushMatrix();

    glTranslated(0.0,0.5,-4.0);
    glutSolidSphere(1.0,10.0,10.0);

    glPopMatrix();

    //les yeux
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glTranslated(0.4,0.6,-4.8);
    glutSolidSphere(0.3,10.0,10.0);

    glPopMatrix();


    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glTranslated(-0.4,0.6,-4.8);
    glutSolidSphere(0.3,10.0,10.0);

    glPopMatrix();
    //nez
    glPushMatrix();
    glTranslated(0.0,-0.090,-4.8);
    glutSolidCube(0.3);
    glPopMatrix();



 glPopMatrix();


  glutSwapBuffers();

}
// on crée une fonction spéciale qui gére la caméra grace aux fléches du clavier
void keyboard(int touche,int x, int y)
{
    switch(touche){

        case GLUT_KEY_UP :
        angley +=2;
        glutPostRedisplay();
        break;

       case GLUT_KEY_LEFT :

        anglex -=2;
        glutPostRedisplay();
        break;
        case GLUT_KEY_DOWN :
        angley-=2;
        glutPostRedisplay();
        break;

        case GLUT_KEY_RIGHT :
        anglex+=2;
        glutPostRedisplay();
        break;
            default:
        break;
        }
}


void clavier(unsigned char touche,int x,int y)
{
  switch(touche) {
//active la lumiére en appuyant sur la touche du clavier o minuscule
  case 'o' :
    lumiereopen();
    break;

//desactive la lumiére en appuyant sur la touche du clavier O majuscule
  case 'O' :
    lumierclose();
    break;
  case 'l':
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glutPostRedisplay();
    break;
  case 'n':
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glutPostRedisplay();
  break;
//en appuyant sur la touche a minuscule du clavier on fait comme animation une translation
  case 'a':
    animate1();
    glutPostRedisplay();
  break;
//en appuyant sur la touche b minuscule du clavier on fait comme animation une translation vers le coté opposée de celui effectuer par a
  case 'b':
    animate2();
    glutPostRedisplay();
  break;
// en cliquant sur la touche Z majuscule du clavier on fait un zoom sur notre objet tortue
  case 'Z':
    zoom-=0.5;
    glutPostRedisplay();
  break;
  // en cliquant sur la touche z minuscule du clavier on fait un dezoom sur notre objet tortue
  case 'z':
    zoom+=0.5;
    glutPostRedisplay();
  break;
//pour arreter l'animation automatique
  case 'c' :
    glutIdleFunc(nullptr);
    glutPostRedisplay();
  break;




  case 27: /* touche ESC */
    exit(0);
  default:
	  break;
  }


}

void souris(int bouton, int etat,int x,int y)
{
  if (bouton == GLUT_LEFT_BUTTON && etat == GLUT_DOWN)
  {
    presse = 1;
    xold = x;
    yold=y;
  }
  if (bouton == GLUT_LEFT_BUTTON && etat == GLUT_UP)
    presse=0;
}

void sourismouv(int x,int y)
  {
    if (presse)
    {
      anglex=anglex+(x-xold);
      angley=angley+(y-yold);
      glutPostRedisplay();
    }

    xold=x;
    yold=y;
  }

void redim(int l,int h)
{
  if (l<h)
    glViewport(0,(h-l)/2,l,l);
  else
    glViewport((l-h)/2,0,h,h);
}



void loadJpegImage(char *fichier)
{
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE *file;
  unsigned char *ligne;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
#ifdef __WIN32
  if (fopen_s(&file,fichier,"rb") != 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
#elif __GNUC__
  if ((file = fopen(fichier,"rb")) == 0)
    {
      fprintf(stderr,"Erreur : impossible d'ouvrir le fichier texture.jpg\n");
      exit(1);
    }
#endif
  jpeg_stdio_src(&cinfo, file);
  jpeg_read_header(&cinfo, TRUE);

  if ((cinfo.image_width!=1920)||(cinfo.image_height!=1280)) {
    fprintf(stdout,"Erreur : l'image doit etre de taille 256x256\n");
    exit(1);
  }
  if (cinfo.jpeg_color_space==JCS_GRAYSCALE) {
    fprintf(stdout,"Erreur : l'image doit etre de type RGB\n");
    exit(1);
  }

  jpeg_start_decompress(&cinfo);
  ligne=image;
  while (cinfo.output_scanline<cinfo.output_height)
    {
      ligne=image+3*1920*cinfo.output_scanline;
      jpeg_read_scanlines(&cinfo,&ligne,1);
    }
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
}
