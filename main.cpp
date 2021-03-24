// Bibliotecas
#include <iostream>
#include <GL/glut.h>
#include <stdlib.h>

// Constantes globais
GLfloat ratio;
static int anguloHorizontal = 0, anguloVertical = 0;
static int mao1 = 0, mao2 = 0;
static int movimentoMao = 5, posicaoAmbiente = 0.0;
static int cor = 1;
GLfloat zoom = 0;
#define	checkImageWidth 64
#define	checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

#ifdef GL_VERSION_1_1
static GLuint texName;
#endif
/*
    A posicao do animal se da pelo angulo em relacao ao ambiente
    Se o animal tiver virado para a direita da tela, ele esta com angulo 0 graus
    Se o animal tiver virado para o topo da tela, ele esta com angulo 90 graus
    Se o animal tiver virado para a esquerda da tela, ele esta com angulo 180 graus
    Se o animal tiver virado para baixo da tela, ele esta com angulo 270 graus
*/
static int posicao = 0;

// Ativar luz 1
void enableLightOne() {
	// Primeira Luz
    glEnable(GL_LIGHT0);

    float light0[4][4] = {
        // Ambiente
        {0.1f, 0.1f, 0.1f, 1.f},
        // Difusa
        {0.8f, 0.8f, 0.8f, 1.f},
        // Especular
        { 1.f,  1.f,  1.f, 1.f },
        // Positicao
        {0.f, 0.f, 1.f, 1.f}
    };

    // Luz, Caracteristica, Endereco da primeira componente
    glLightfv(GL_LIGHT0, GL_AMBIENT,  &light0[0][0]);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  &light0[1][0]);
    glLightfv(GL_LIGHT0, GL_SPECULAR, &light0[2][0]);
    glLightfv(GL_LIGHT0, GL_POSITION, &light0[3][0]);
}

// Ativar luz 2
void enableLightTwo() {
    // Segunda Luz
    glEnable(GL_LIGHT1);

    float light1[4][4] = {
        // Ambiente
        {0.1f, 0.3f, 0.3f, 3.f},
        // Difusa
        {0.8f, 0.5f, 0.5f, 1.f},
        // Especular
        { 1.f,  1.f,  1.f, 1.f },
        // Positicao
        {1.f, 0.f, 0.f, 1.f}
    };

    // Luz, Caracteristica, Endereco da primeira componente
    glLightfv(GL_LIGHT1, GL_AMBIENT,  &light1[0][0]);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  &light1[1][0]);
    glLightfv(GL_LIGHT1, GL_SPECULAR, &light1[2][0]);
    glLightfv(GL_LIGHT1, GL_POSITION, &light1[3][0]);
}

// Desativar luz 1
void disableLightOne() {
    glDisable(GL_LIGHT0);
}

// Desativar luz 2
void disableLightTwo() {
    glDisable(GL_LIGHT1);
}
void makeCheckImage(void)
{
   int i, j, c;
    
   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         c = ((((i&0x8)==0)^((j&0x8))==0))*255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c; // brilho do antebraço e cabeça
         checkImage[i][j][2] = (GLubyte) c;
         checkImage[i][j][3] = (GLubyte) 255;
      }
   }
}

// Inicializa os parametros globais de OpenGL
void init(void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glShadeModel(GL_SMOOTH);						
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);	
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    // Luz Ambiente
    float globalAmb[] = { 0.1f, 0.1f, 0.1f, 1.f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmb);
    
    makeCheckImage();
   glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

#ifdef GL_VERSION_1_1
   glGenTextures(2, &texName);
   glBindTexture(GL_TEXTURE_2D, texName);
#endif

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

   glTexImage2D(GL_TEXTURE_2D, 0, 4, checkImageWidth, checkImageHeight, 
                0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

}

// Posicao do usuario
void PosicaoUser() {
    // Define o volume de recorte
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90,ratio,0.01,200);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        0.0f, 0.0f, -0.0f,
        0.0f, 0.0f, -8.0f,
        0.0f, 1.0f, 0.0f
    );
}

// Trata o redimensionamento da janela OpenGL
void reshape(int w, int h) {
    // Previne a divisao por zero, quando a imagem eh muito pequena
    // Nao eh posicao fazer uma janela de largura zero
    if (h == 0) {
        h = 1;
    }

    ratio = 1.0f * w / h;

    // Reseta o sistema de coordenadas antes de moficiar
    glMatrixMode(GL_PROJECTION);

    // Seta o viewport para seja a janela inteira
    glViewport(0, 0, w, h);

    PosicaoUser();
}

// Funcao que desenha um cubo
// Serve de axiliar para realizar os desenhos
void DesenharCubo() {
	glBegin(GL_QUADS);
		// Face frontal
		glNormal3f(0, 0, 1);
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(0.0, 1.0); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0, 1.0); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0, 0.0); glVertex3f(-1.0f, 1.0f, 1.0f);
		// Face de tras
		glNormal3f(0, 0, -1);
		glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(1.0, 1.0);glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(1.0, 0.0);glVertex3f(1.0f, -1.0f, -1.0f);
		// Face de cima
		glNormal3f(0, 1, 0);
		glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0, 1.0);glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0, 0.0);glVertex3f(1.0f, 1.0f, -1.0f);
		// Face de baixo
		glNormal3f(0, -1, 0);
		glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0, 1.0);glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0, 1.0);glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, -1.0f, 1.0f);
		// Face da direita
		glNormal3f(1,0,0);
		glTexCoord2f(0.0, 0.0);glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0, 1.0);glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(1.0, 1.0);glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0, 0.0);glVertex3f(1.0f, -1.0f, 1.0f);
		// Face da esquerda
		glNormal3f(-1, 0, 0);
		glTexCoord2f(0.0, 0.0);glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0, 1.0);glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1.0, 1.0);glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1.0, 0.0);glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
}

void selecionarCor(int cor) {
    switch (cor) {
        case 1:
            glColor3f(0.0f, 1.0f, 0.0f);
            break;
        case 2:
            glColor3f(0.0f, 0.0f, 1.0f);
            break;
        case 3:
            glColor3f(1.0f, 0.0f, 0.0f);
            break;
        case 4:
            glColor3f(1.0f, 1.0f, 0.0f);
            break;
        default:
            break;
    }
}

void desenhaAmbiente() {
    glPushMatrix();
        glTranslatef(-posicaoAmbiente, 0.0f, 0.0f);
        glScalef(100.0f, 100.0f, 1.0f);
        glTranslatef(0.0f, 0.0f, -40.0f);
        selecionarCor(4);
        DesenharCubo();
        glTranslatef(2.0f, 0.0f, 0.0f);
        selecionarCor(3);
        DesenharCubo();
        glTranslatef(2.0f, 0.0f, 0.0f);
        selecionarCor(2);
        DesenharCubo();
        glTranslatef(2.0f, 0.0f, 0.0f);
        selecionarCor(1);
        DesenharCubo();
        glScalef(0.01f, 0.01f, 1.0f);
        glTranslatef(posicaoAmbiente, 0.0f, 40.0f);
    glPopMatrix();
}

void desenharMaoDireitaDianteira() {
    glTranslatef(4.0f, 3.0f, -10.0f);
    glRotatef((GLfloat) mao1, 1.0, 0.0, 0.0);
    glTranslatef(-4.0f, -3.0f, 10.0f);
    glTranslatef(4.25f, 3.0f, -11.0f);
    glScalef(0.25f, 0.5f, 1.0f);
    selecionarCor(cor);
    DesenharCubo();
    glScalef(4.0f, 2.0f, 1.0f);
    glTranslatef(-4.25f, -3.0f, 11.0f);
}

void desenharMaoEsquerdaDianteira() {
    glTranslatef(4.0f, 3.0f, -10.0f);
    glRotatef((GLfloat) mao2, 1.0, 0.0, 0.0);
    glTranslatef(-4.0f, -3.0f, 10.0f);
    glTranslatef(-4.25f, 3.0f, -11.0f);
    glScalef(0.25f, 0.5f, 1.0f);
    selecionarCor(cor);
    DesenharCubo();
    glScalef(4.0f, 2.0f, 1.0f);
    glTranslatef(4.25f, -3.0f, 11.0f);
}

void desenharMaoEsquerdaTraseira() {
    glTranslatef(3.0f, -3.0f, -10.0f);
    glRotatef((GLfloat) mao2, 1.0, 0.0, 0.0);
    glTranslatef(-3.0f, 3.0f, 10.0f);
    glTranslatef(-4.25f, -3.0f, -11.0f);
    glScalef(0.25f, 0.5f, 1.0f);
    selecionarCor(cor);
    DesenharCubo();
    glScalef(4.0f, 2.0f, 1.0f);
    glTranslatef(4.25f, 3.0f, 11.0f);
}

void desenharMaoDireitaTraseira() {
    glTranslatef(3.0f, -3.0f, -10.0f);
    glRotatef((GLfloat) mao1, 1.0, 0.0, 0.0);
    glTranslatef(-3.0f, 3.0f, 10.0f);
    glTranslatef(4.25f, -3.0f, -11.0f);
    glScalef(0.25f, 0.5f, 1.0f);
    selecionarCor(cor);
    DesenharCubo();
    glScalef(4.0f, 2.0f, 1.0f);
    glTranslatef(-4.25f, 3.0f, 11.0f);
}

void desenharBracoDireitoTraseiro() {
    glTranslatef(3.0f, -3.0f, -10.0f);
    glScalef(1.0f, 0.5f, 0.25f);
    selecionarCor(cor);
    DesenharCubo();
    glScalef(1.0f, 2.0f, 4.0f);
    glTranslatef(-3.0f, 3.0f, 10.0f);
}

void desenharBracoEsquerdoTraseiro() {
    glTranslatef(-3.0f, -3.0f, -10.0f);
    glScalef(1.0f, 0.5f, 0.25f);
    selecionarCor(cor);
    DesenharCubo();
    glScalef(1.0f, 2.0f, 4.0f);
    glTranslatef(3.0f, 3.0f, 10.0f);
}

void desenharBracoEsquerdoDianteiro() {
    glTranslatef(-3.0f, 3.0f, -10.0f);
    glScalef(1.0f, 0.5f, 0.25f);
    selecionarCor(cor);
    DesenharCubo();
    glScalef(1.0f, 2.0f, 4.0f);
    glTranslatef(3.0f, -3.0f, 10.0f);
}

void desenharBracoDireitoDianteiro() {
    glTranslatef(3.0f, 3.0f, -10.0f);
    glScalef(1.0f, 0.5f, 0.25f);
    selecionarCor(cor);
    DesenharCubo();
    glScalef(1.0f, 2.0f, 4.0f);
    glTranslatef(-3.0f, -3.0f, 10.0f);
}

void desenharCabeca() {
    glTranslatef(0.0f, 5.0f, -10.0f);
    glScalef(1.0f, 1.0f, 0.5f);
    selecionarCor(cor);
    DesenharCubo();
    glScalef(1.0f, 1.0f, 2.0f);
    glTranslatef(0.0f, -5.0f, 10.0f);
}

void desenharTronco() {
    glTranslatef(0.0f, 0.0f, -10.0f);
    glScalef(2.0f, 4.0f, 1.0f);
    selecionarCor(cor);
    DesenharCubo();
    glScalef(0.5f, 0.25f, 1.0f);
    glTranslatef(0.0f, 0.0f, 10.0f);
}

void moverAnimalVertical() {
    glTranslatef(0.0f, 0.0f, -10.0f);
    glRotatef(-90, 0, 1, 0);
    glRotatef(-90, 1, 0, 0);
    glTranslatef(0.0f, 0.0f, 10.0f);
}

void rotacionarDesenho() {
    glTranslatef(0.0f, 0.0f, -10.0f);
    glRotatef((GLfloat) posicao, 0.0, 1.0, 0.0);
    glTranslatef(0.0f, 0.0f, 10.0f);
}

void desenhaCamaleao() {
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        rotacionarDesenho();
        glPushMatrix();
            moverAnimalVertical();
            desenharTronco();
            glPushMatrix();
                desenharCabeca();
                glPushMatrix();
                    desenharBracoDireitoDianteiro();
                    glPushMatrix();
                        desenharMaoDireitaDianteira();
                    glPopMatrix();
                glPopMatrix();
                glPushMatrix();
                    desenharBracoEsquerdoDianteiro();
                    glPushMatrix();
                        desenharMaoEsquerdaDianteira();
                    glPopMatrix();
                glPopMatrix();
                glPushMatrix();
                    desenharBracoEsquerdoTraseiro();
                    glPushMatrix();
                        desenharMaoEsquerdaTraseira();
                    glPopMatrix();
                glPopMatrix();
                glPushMatrix();
                    desenharBracoDireitoTraseiro();
                    glPushMatrix();
                        desenharMaoDireitaTraseira();
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
    glFlush();
    glDisable(GL_TEXTURE_2D);
}

// Funcao que realizar os desenhos
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	PosicaoUser();
	glTranslatef(0.0f, 0.0f, zoom);
	glMatrixMode(GL_MODELVIEW);
	
    desenhaAmbiente();
    
    desenhaCamaleao();
	
	glutSwapBuffers();
}

void aumentarZoom(float i) {
    zoom += i;
    display();
}

void diminuirZoom(float i) {
    zoom -= i;
    display();
}

// Funcao que faz o movimento do mouse
void mouse(int button, int state, int mousex, int mousey) { 
  if(button==GLUT_LEFT_BUTTON){
  	aumentarZoom(0.3);
  }
  else if(button==GLUT_RIGHT_BUTTON){
  	diminuirZoom(0.3);
  }
}

// Funcao que realiza o movimento das maos
void movimentoMaos(void) {
    if (mao1 == 30) {
        movimentoMao = -5;
    }
    if (mao1 == -30) {
        movimentoMao = 5;
    }
    mao1 = (mao1 + movimentoMao) % 360;
    mao2 = (mao2 - movimentoMao) % 360;
}

// Verifica a posicao do animal
// A posicao vai de 0 graus ate 360 graus
void verificarPosicao(void) {
    if (posicao < 0) {
        posicao = 360;
    } else if(posicao > 360) {
        posicao = 0;
    }
}

// Funcao que gira o animal no sentido horario
void girarAnimalHorario(void) {
    posicao = posicao - 1;
    verificarPosicao();
}

// Funcao que gira o animal no sentido anti-horario
void girarAnimalAntiHorario(void) {
    posicao = posicao + 1;
    verificarPosicao();
}

// Funcao que movimenta o animal na ida do ambiente
void movimentoAmbienteIda(void) {
    posicaoAmbiente = posicaoAmbiente + 1;
}

// Funcao que movimenta o animal na volta do ambiente
void movimentoAmbienteVolta(void) {
    posicaoAmbiente = posicaoAmbiente - 1;
}

void movimentarParaFrente() {
    if (posicao < 90 || (posicao > 270)) {
        girarAnimalAntiHorario();
    } else {
        girarAnimalHorario();
    }
    if (posicao > 80 && posicao < 100) {
        diminuirZoom(0.05);
    }
    movimentoMaos();
}

void movimentarParaEsquerda() {
    if (posicao == 180) {
        movimentoAmbienteVolta();
    } else if (posicao < 180) {
        girarAnimalAntiHorario();
    } else if (posicao <= 360) {
        girarAnimalHorario();
    }
    movimentoMaos();
}

void movimentarParaDireita() {
    if (posicao < 180) {
        girarAnimalHorario();
    } else {
        girarAnimalAntiHorario();
    }
    if (posicao < 5 || posicao > 355) {
        movimentoAmbienteIda();
    }
    movimentoMaos();
}

void movimentarParaBaixo() {
    if (posicao < 90 || posicao > 270) {
        girarAnimalHorario();
    } else {
        girarAnimalAntiHorario();
    }
    if (posicao > 260 && posicao < 280 && zoom < 1.1) {
        aumentarZoom(0.05);
    }
    movimentoMaos();
}

// Funcao que controla o teclado
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        // Termina o problema quando a tecla ESC for pressionada
        case 27:
            exit(0);
            break;
        // Primeira cor (cor original)
        case '1':
            cor = 1;
            break;
        // Segunda cor
        case '2':
            cor = 2;
            break;
        // Terceira cor
        case '3':
            cor = 3;
            break;
        // Quarta cor
        case '4':
            cor = 4;
            break;
        // Movimenta o animal para frente
        case 'w':
            movimentarParaFrente();
            glutPostRedisplay();
            break;
        // Movimenta o animal para esquerda
        case 'a':
            movimentarParaEsquerda();
            glutPostRedisplay();
            break;
        // Movimenta o animal para direita
        case 'd':
            movimentarParaDireita();
            glutPostRedisplay();
            break;
        // Movimenta o animal para baixo
        case 's':
            movimentarParaBaixo();
            glutPostRedisplay();
            break;
        case 'z':
            enableLightOne();
            break;
        case 'x':
            enableLightTwo();
            break;
        case 'c':
            disableLightOne();
            break;
        case 'v':
            disableLightTwo();
            break;
        default:        
            break;
    }
}

void arrow_keys (int a_keys, int x, int y) {
    switch (a_keys) {
        // Quando a tecla para cima for pressionada
        case GLUT_KEY_UP:
            // Vai para o modo de tela inteira
            glutFullScreen();
            break;
        // Quando a tecla para baico for pressionada
        case GLUT_KEY_DOWN:
            glutInitWindowSize(700, 500);
            break;
        default:
            break;
    }
}

int main(int argc, char** argv) {
   glutInit(&argc, argv); 
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
   glutInitWindowPosition(0,0);
   glutInitWindowSize(800, 600); 
   glutCreateWindow("GCC124 - Trabalho"); 
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(arrow_keys);
   glutIdleFunc(display);
  glutMouseFunc(mouse);
   glutMainLoop();
   return 0;       
}

