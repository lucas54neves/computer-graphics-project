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
/*
    A posicao do animal se da pelo angulo em relacao ao ambiente
    Se o animal tiver virado para a direita da tela, ele esta com angulo 0 graus
    Se o animal tiver virado para o topo da tela, ele esta com angulo 90 graus
    Se o animal tiver virado para a esquerda da tela, ele esta com angulo 180 graus
    Se o animal tiver virado para baixo da tela, ele esta com angulo 270 graus
*/
static int posicao = 0;

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
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		// Face de tras
		glNormal3f(0, 0, -1);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		// Face de cima
		glNormal3f(0, 1, 0);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		// Face de baixo
		glNormal3f(0, -1, 0);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		// Face da direita
		glNormal3f(1,0,0);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		// Face da esquerda
		glNormal3f(-1, 0, 0);
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
}

// Funcao que realizar os desenhos
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	PosicaoUser();
	
	glMatrixMode(GL_MODELVIEW);

    // Desenho do ambiente
    glPushMatrix();
        glTranslatef(-posicaoAmbiente, 0.0f, 0.0f);
        glScalef(100.0f, 100.0f, 1.0f);

        glTranslatef(0.0f, 0.0f, -40.0f);
        // Cinza
        glColor3f(0.5f, 0.5f, 0.5f);
        DesenharCubo();

        glTranslatef(2.0f, 0.0f, 0.0f);
        // Camuflagem azul
        glColor3f(0.0f, 0.0f, 1.0f);
        DesenharCubo();

        glTranslatef(2.0f, 0.0f, 0.0f);
        // Camuflagem vermelha
        glColor3f(1.0f, 0.0f, 0.0f);
        DesenharCubo();

        glTranslatef(2.0f, 0.0f, 0.0f);
        // Camuflagem amarelo
        glColor3f(1.0f, 1.0f, 0.0f);
        DesenharCubo();

        glScalef(0.01f, 0.01f, 1.0f);
        glTranslatef(posicaoAmbiente, 0.0f, 40.0f);
    glPopMatrix();

    // Desenho do animal
    glPushMatrix();
        // Rotaciona o desenho
        glTranslatef(0.0f, 0.0f, -10.0f);
        glRotatef((GLfloat) posicao, 0.0, 1.0, 0.0);
        glTranslatef(0.0f, 0.0f, 10.0f);

        // Desenho do camaleao
        glPushMatrix();
            // Coloca o animal na vertical
            glTranslatef(0.0f, 0.0f, -10.0f);
            glRotatef(-90, 0, 1, 0);
            glRotatef(-90, 1, 0, 0);
            glTranslatef(0.0f, 0.0f, 10.0f);

            // Tronco do animal
            glTranslatef(0.0f, 0.0f, -10.0f);
            glScalef(2.0f, 4.0f, 1.0f);
            
            // Selecao das cores
            switch (cor) {
                // Cor original
                case 1:
                    glColor3f(0.0f, 0.0f, 1.0f);
                    break;
                // Camuflagem azul
                case 2:
                    glColor3f(0.0f, 0.0f, 1.0f);
                    break;
                // Camuflagem vermelha
                case 3:
                    glColor3f(1.0f, 0.0f, 0.0f);
                    break;
                // Camuflagem amarelo
                case 4:
                    glColor3f(1.0f, 1.0f, 0.0f);
                    break;
                default:
                    break;
            }
            
            DesenharCubo();
            glScalef(0.5f, 0.25f, 1.0f);
            glTranslatef(0.0f, 0.0f, 10.0f);

            glPushMatrix();
                // Cabeca do animal
                glTranslatef(0.0f, 5.0f, -10.0f);
                glScalef(1.0f, 1.0f, 0.5f);
            
                // Selecao das cores
                switch (cor) {
                    // Cor original
                    case 1:
                        glColor3f(0.0f, 1.0f, 0.0f);
                        break;
                    // Camuflagem azul
                    case 2:
                        glColor3f(0.0f, 0.0f, 1.0f);
                        break;
                    // Camuflagem vermelha
                    case 3:
                        glColor3f(1.0f, 0.0f, 0.0f);
                        break;
                    // Camuflagem amarelo
                    case 4:
                        glColor3f(1.0f, 1.0f, 0.0f);
                        break;
                    default:
                        break;
                }
            
                DesenharCubo();
                glScalef(1.0f, 1.0f, 2.0f);
                glTranslatef(0.0f, -5.0f, 10.0f);

                glPushMatrix();
                    // Braco dianteiro da direita
                    glTranslatef(3.0f, 3.0f, -10.0f);
                    glScalef(1.0f, 0.5f, 0.25f);
            
                    // Selecao das cores
                    switch (cor) {
                        // Cor original
                        case 1:
                            glColor3f(0.0f, 1.0f, 0.0f);
                            break;
                        // Camuflagem azul
                        case 2:
                            glColor3f(0.0f, 0.0f, 1.0f);
                            break;
                        // Camuflagem vermelha
                        case 3:
                            glColor3f(1.0f, 0.0f, 0.0f);
                            break;
                        // Camuflagem amarelo
                        case 4:
                            glColor3f(1.0f, 1.0f, 0.0f);
                            break;
                        default:
                            break;
                    }
            
                    DesenharCubo();
                    glScalef(1.0f, 2.0f, 4.0f);
                    glTranslatef(-3.0f, -3.0f, 10.0f);

                    glPushMatrix();
                        // Movimento da mao
                        glTranslatef(4.0f, 3.0f, -10.0f);
                        glRotatef((GLfloat) mao1, 1.0, 0.0, 0.0);
                        glTranslatef(-4.0f, -3.0f, 10.0f);

                        // Desenho da mao
                        glTranslatef(4.25f, 3.0f, -11.0f);
                        glScalef(0.25f, 0.5f, 1.0f);
            
                        // Selecao das cores
                        switch (cor) {
                            // Cor original
                            case 1:
                                glColor3f(0.0f, 0.0f, 1.0f);
                                break;
                            // Camuflagem azul
                            case 2:
                                glColor3f(0.0f, 0.0f, 1.0f);
                                break;
                            // Camuflagem vermelha
                            case 3:
                                glColor3f(1.0f, 0.0f, 0.0f);
                                break;
                            // Camuflagem amarelo
                            case 4:
                                glColor3f(1.0f, 1.0f, 0.0f);
                                break;
                            default:
                                break;
                        }
            
                        DesenharCubo();
                        glScalef(4.0f, 2.0f, 1.0f);
                        glTranslatef(-4.25f, -3.0f, 11.0f);
                    glPopMatrix();
                glPopMatrix();

                glPushMatrix();
                    // Braco dianteiro da esquerda
                    glTranslatef(-3.0f, 3.0f, -10.0f);
                    glScalef(1.0f, 0.5f, 0.25f);
            
                    // Selecao das cores
                    switch (cor) {
                        // Cor original
                        case 1:
                            glColor3f(0.0f, 1.0f, 0.0f);
                            break;
                        // Camuflagem azul
                        case 2:
                            glColor3f(0.0f, 0.0f, 1.0f);
                            break;
                        // Camuflagem vermelha
                        case 3:
                            glColor3f(1.0f, 0.0f, 0.0f);
                            break;
                        // Camuflagem amarelo
                        case 4:
                            glColor3f(1.0f, 1.0f, 0.0f);
                            break;
                        default:
                            break;
                    }
                    
                    DesenharCubo();
                    glScalef(1.0f, 2.0f, 4.0f);
                    glTranslatef(3.0f, -3.0f, 10.0f);

                    glPushMatrix();
                        // Movimento da mao
                        glTranslatef(4.0f, 3.0f, -10.0f);
                        glRotatef((GLfloat) mao2, 1.0, 0.0, 0.0);
                        glTranslatef(-4.0f, -3.0f, 10.0f);

                        // Desenho da mao
                        glTranslatef(-4.25f, 3.0f, -11.0f);
                        glScalef(0.25f, 0.5f, 1.0f);
            
                        // Selecao das cores
                        switch (cor) {
                            // Cor original
                            case 1:
                                glColor3f(0.0f, 0.0f, 1.0f);
                                break;
                            // Camuflagem azul
                            case 2:
                                glColor3f(0.0f, 0.0f, 1.0f);
                                break;
                            // Camuflagem vermelha
                            case 3:
                                glColor3f(1.0f, 0.0f, 0.0f);
                                break;
                            // Camuflagem amarelo
                            case 4:
                                glColor3f(1.0f, 1.0f, 0.0f);
                                break;
                            default:
                                break;
                        }
                        
                        DesenharCubo();
                        glScalef(4.0f, 2.0f, 1.0f);
                        glTranslatef(4.25f, -3.0f, 11.0f);
                    glPopMatrix();
                glPopMatrix();

                glPushMatrix();
                    // Braco traseira da esquerda
                    glTranslatef(-3.0f, -3.0f, -10.0f);
                    glScalef(1.0f, 0.5f, 0.25f);
            
                    // Selecao das cores
                    switch (cor) {
                        // Cor original
                        case 1:
                            glColor3f(0.0f, 1.0f, 0.0f);
                            break;
                        // Camuflagem azul
                        case 2:
                            glColor3f(0.0f, 0.0f, 1.0f);
                            break;
                        // Camuflagem vermelha
                        case 3:
                            glColor3f(1.0f, 0.0f, 0.0f);
                            break;
                        // Camuflagem amarelo
                        case 4:
                            glColor3f(1.0f, 1.0f, 0.0f);
                            break;
                        default:
                            break;
                    }
                    
                    DesenharCubo();
                    glScalef(1.0f, 2.0f, 4.0f);
                    glTranslatef(3.0f, 3.0f, 10.0f);

                    glPushMatrix();
                        // Movimento da mao
                        glTranslatef(3.0f, -3.0f, -10.0f);
                        glRotatef((GLfloat) mao2, 1.0, 0.0, 0.0);
                        glTranslatef(-3.0f, 3.0f, 10.0f);

                        // Desenho da mao
                        glTranslatef(-4.25f, -3.0f, -11.0f);
                        glScalef(0.25f, 0.5f, 1.0f);
            
                        // Selecao das cores
                        switch (cor) {
                            // Cor original
                            case 1:
                                glColor3f(0.0f, 0.0f, 1.0f);
                                break;
                            // Camuflagem azul
                            case 2:
                                glColor3f(0.0f, 0.0f, 1.0f);
                                break;
                            // Camuflagem vermelha
                            case 3:
                                glColor3f(1.0f, 0.0f, 0.0f);
                                break;
                            // Camuflagem amarelo
                            case 4:
                                glColor3f(1.0f, 1.0f, 0.0f);
                                break;
                            default:
                                break;
                        }
                        
                        DesenharCubo();
                        glScalef(4.0f, 2.0f, 1.0f);
                        glTranslatef(4.25f, 3.0f, 11.0f);
                    glPopMatrix();
                glPopMatrix();

                glPushMatrix();
                    // Braco traseira da direita
                    glTranslatef(3.0f, -3.0f, -10.0f);
                    glScalef(1.0f, 0.5f, 0.25f);
            
                    // Selecao das cores
                    switch (cor) {
                        // Cor original
                        case 1:
                            glColor3f(0.0f, 1.0f, 0.0f);
                            break;
                        // Camuflagem azul
                        case 2:
                            glColor3f(0.0f, 0.0f, 1.0f);
                            break;
                        // Camuflagem vermelha
                        case 3:
                            glColor3f(1.0f, 0.0f, 0.0f);
                            break;
                        // Camuflagem amarelo
                        case 4:
                            glColor3f(1.0f, 1.0f, 0.0f);
                            break;
                        default:
                            break;
                    }
                    
                    DesenharCubo();
                    glScalef(1.0f, 2.0f, 4.0f);
                    glTranslatef(-3.0f, 3.0f, 10.0f);

                    glPushMatrix();
                        // Movimento da mao
                        glTranslatef(3.0f, -3.0f, -10.0f);
                        glRotatef((GLfloat) mao1, 1.0, 0.0, 0.0);
                        glTranslatef(-3.0f, 3.0f, 10.0f);

                        // Desenho da mao
                        glTranslatef(4.25f, -3.0f, -11.0f);
                        glScalef(0.25f, 0.5f, 1.0f);
            
                        // Selecao das cores
                        switch (cor) {
                            // Cor original
                            case 1:
                                glColor3f(0.0f, 0.0f, 1.0f);
                                break;
                            // Camuflagem azul
                            case 2:
                                glColor3f(0.0f, 0.0f, 1.0f);
                                break;
                            // Camuflagem vermelha
                            case 3:
                                glColor3f(1.0f, 0.0f, 0.0f);
                                break;
                            // Camuflagem amarelo
                            case 4:
                                glColor3f(1.0f, 1.0f, 0.0f);
                                break;
                            default:
                                break;
                        }
                        
                        DesenharCubo();
                        glScalef(4.0f, 2.0f, 1.0f);
                        glTranslatef(-4.25f, 3.0f, 11.0f);
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();

	glutSwapBuffers();
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
            if (posicao < 90 || (posicao > 270)) {
                girarAnimalAntiHorario();
            } else {
                girarAnimalHorario();
            }
            movimentoMaos();
            glutPostRedisplay();
            break;
        // Movimenta o animal para esquerda
        case 'a':
            if (posicao == 180) {
                movimentoAmbienteVolta();
            } else if (posicao < 180) {
                girarAnimalAntiHorario();
            } else if (posicao <= 360) {
                girarAnimalHorario();
            }
            movimentoMaos();
            glutPostRedisplay();
            break;
        // Movimenta o animal para direita
        case 'd':
            if (posicao < 180) {
                girarAnimalHorario();
            } else {
                girarAnimalAntiHorario();
            }
            if (posicao < 5 || posicao > 355) {
                movimentoAmbienteIda();
            }
            movimentoMaos();
            glutPostRedisplay();
            break;
        // Movimenta o animal para baixo
        case 's':
            if (posicao < 90 || posicao > 270) {
                girarAnimalHorario();
            } else {
                girarAnimalAntiHorario();
            }
            movimentoMaos();
            glutPostRedisplay();
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
   glutMainLoop();
   return 0;       
}