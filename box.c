#include "chConsoleFunctions.h"
#include "coisas.h"

int main()
{
    _cs_infoEx csinfo;
    _cs_status console_antes;
    _cs_status* antes = &console_antes;
    char    color = 1;
    char    comando = '@';
    char    espacos[71];
    int     i = 0;
    char    linha[50];
    int     slice = 1000;
    csinfo.log = NULL;
    antes->log = NULL;
    memset(espacos, 32, 70);
    espacos[70] = 0;
    antes = guarda(antes);
    if ((prepara_tela(&csinfo)) < 0)
    {
        restaura(antes);
        return -1;
    };

    int msg_line = csinfo.l + 2;
    int nova = -1;
    while (comando != 'q')
    {
        box(&csinfo);
        text_color(_verde_, _preto_);
        gotoYX(msg_line, 6);
        printf("Tecle h para help, comando ou ENTER ");
        comando = choice();
        gotoYX(msg_line, 6);
        switch (comando)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
            csinfo.estilo = comando - '0';
            break;
        case 'C':
        case 'c':
            while ( (nova = rand() % 16) == csinfo.cor);
            csinfo.cor = nova;
            if (csinfo.cor == 0) csinfo.cor = 1;
            break;
        case 'h':
        case 'H':
            gotoYX(msg_line, 6);
            printf("'c' cor '0' a '5' estilo 't' tempo. 'q' encerra. Tecle ENTER ");
            gotoYX(msg_line, 6);
            fgets(linha, 5, stdin);
            gotoYX(msg_line, 6);
            printf("%s", espacos);
            break;
        case 'q':
        case 'Q':
            restaura(antes);
            cls();
            return 0;
            break;
        case 't':
        case 'T':
            gotoYX(msg_line, 6);
            printf("%60s", espacos);
            gotoYX(msg_line, 6);
            printf("[%d] Novo tempo em 1/10s: ", slice/100);
            fgets(linha, 5, stdin);
            int novo = atoi(linha);
            if ((novo >= 1) && (novo <= 1000 )) slice = novo*100;
            gotoYX(msg_line, 6);
            printf("%60s", espacos);
            break;
        default:
            break;
        };  // switch()
        Sleep(slice);
    };  // while()
    restaura(antes);
    cls();
    return 0;
};  // main()

// fim