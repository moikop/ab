#define LETRA_A 'a'
#define LETRA_B 'b'
#define LETRA_C 'c'
#define LETRA_D 'd'
#define LETRA_E 'e'
#define LETRA_F 'f'
#define LETRA_G 'g'
#define LETRA_H 'h'
#define LETRA_I 'i'
#define LETRA_J 'j'
#define LETRA_K 'k'
#define LETRA_L 'l'
#define LETRA_M 'm'
#define LETRA_N 'n'
#define LETRA_O 'o'
#define LETRA_P 'p'
#define LETRA_Q 'q'
#define LETRA_R 'r'
#define LETRA_S 's'
#define LETRA_T 't'
#define LETRA_U 'u'
#define LETRA_V 'v'
#define LETRA_W 'w'
#define LETRA_X 'x'
#define LETRA_Y 'y'
#define LETRA_Z 'z'
#define NUM_0 '0'
#define NUM_1 '1'
#define NUM_2 '2'
#define NUM_3 '3'
#define NUM_4 '4'
#define NUM_5 '5'
#define NUM_6 '6'
#define NUM_7 '7'
#define NUM_8 '8'
#define NUM_9 '9'
#ifndef
    #define NO_HAY_MJE -1
#endif
#ifndef
    #define CARACTER_ERRONEO -2
#endif

int Encriptar(char* mje)
{
    int i=0;
    if (!mje[0])
        return NO_HAY_MJE;
    while (mje[i]<>/0)  /*VER!!!*/
    {
        switch(mje[i])
        {
            case LETRA_A: mje[i]=NUM_0;
            case LETRA_B: mje[i]=NUM_9;
            case LETRA_C: mje[i]=NUM_6;
            case LETRA_D: mje[i]=NUM_3;
            case LETRA_E: mje[i]=NUM_8;
            case LETRA_F: mje[i]=NUM_5;
            case LETRA_G: mje[i]=NUM_2;
            case LETRA_H: mje[i]=NUM_7;
            case LETRA_I: mje[i]=NUM_4;
            case LETRA_J: mje[i]=NUM_1;
            case LETRA_K: mje[i]=LETRA_H;
            case LETRA_L: mje[i]=LETRA_B;
            case LETRA_M: mje[i]=LETRA_C;
            case LETRA_N: mje[i]=LETRA_Q;
            case LETRA_O: mje[i]=LETRA_E;
            case LETRA_P: mje[i]=LETRA_Y;
            case LETRA_Q: mje[i]=LETRA_G;
            case LETRA_R: mje[i]=LETRA_A;
            case LETRA_S: mje[i]=LETRA_M;
            case LETRA_T: mje[i]=LETRA_J;
            case LETRA_U: mje[i]=LETRA_K;
            case LETRA_V: mje[i]=LETRA_L;
            case LETRA_W: mje[i]=LETRA_I;
            case LETRA_X: mje[i]=LETRA_V;
            case LETRA_Y: mje[i]=LETRA_O;
            case LETRA_Z: mje[i]=LETRA_P;
            case NUM_0: mje[i]=LETRA_D;
            case NUM_1: mje[i]=LETRA_R;
            case NUM_2: mje[i]=LETRA_S;
            case NUM_3: mje[i]=LETRA_T;
            case NUM_4: mje[i]=LETRA_U;
            case NUM_5: mje[i]=LETRA_N;
            case NUM_6: mje[i]=LETRA_W;
            case NUM_7: mje[i]=LETRA_X;
            case NUM_8: mje[i]=LETRA_F;
            case NUM_9: mje[i]=LETRA_Z;
            default: return CARACTER_ERRONEO;
        }
        i++;
    }
    return RES_OK;
}

int Desencriptar(char* mje);
{
    int i=0;
    if (!mje[i])
        return NO_HAY_MJE;
    while (mje[i]<>/0)  /*VER!!!*/
    {
        switch(mje[i])
        {
            case LETRA_A: mje[i]=LETRA_R;
            case LETRA_B: mje[i]=LETRA_L;
            case LETRA_C: mje[i]=LETRA_M;
            case LETRA_D: mje[i]=NUM_0;
            case LETRA_E: mje[i]=LETRA_O;
            case LETRA_F: mje[i]=NUM_8;
            case LETRA_G: mje[i]=LETRA_Q;
            case LETRA_H: mje[i]=LETRA_K;
            case LETRA_I: mje[i]=LETRA_W;
            case LETRA_J: mje[i]=LETRA_T;
            case LETRA_K: mje[i]=LETRA_U;
            case LETRA_L: mje[i]=LETRA_V;
            case LETRA_M: mje[i]=LETRA_S;
            case LETRA_N: mje[i]=NUM_5;
            case LETRA_O: mje[i]=LETRA_Y;
            case LETRA_P: mje[i]=LETRA_Z;
            case LETRA_Q: mje[i]=LETRA_N;
            case LETRA_R: mje[i]=NUM_1;
            case LETRA_S: mje[i]=NUM_2;
            case LETRA_T: mje[i]=NUM_3;
            case LETRA_U: mje[i]=NUM_4;
            case LETRA_V: mje[i]=LETRA_X;
            case LETRA_W: mje[i]=NUM_6;
            case LETRA_X: mje[i]=NUM_7;
            case LETRA_Y: mje[i]=LETRA_P;
            case LETRA_Z: mje[i]=NUM_9;
            case NUM_0: mje[i]=LETRA_A;
            case NUM_1: mje[i]=LETRA_J;
            case NUM_2: mje[i]=LETRA_G;
            case NUM_3: mje[i]=LETRA_D;
            case NUM_4: mje[i]=LETRA_I;
            case NUM_5: mje[i]=LETRA_F;
            case NUM_6: mje[i]=LETRA_C;
            case NUM_7: mje[i]=LETRA_H;
            case NUM_8: mje[i]=LETRA_E;
            case NUM_9: mje[i]=LETRA_B;
            default: return CARACTER_ERRONEO;
        }
    }
    return RES_OK;
}
