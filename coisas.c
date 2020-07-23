#include "chConsoleFunctions.h"

int         box(_cs_infoEx* info)
{
    char topbar[256];
    char intbar[256];
    char lines[6][6] = {
        { 192, 196, 217, 179, 191, 218 },
        { 200, 205, 188, 186, 187, 201 },
        { 176, 176, 176, 176, 176, 176 },
        { 177, 177, 177, 177, 177, 177 },
        { 178, 178, 178, 178, 178, 178 },
        { 219, 219, 219, 219, 219, 219 }
    };
    char* line = &lines[info->estilo][0];
    // define posicao do box
    int ol = rand() % (info->l - 3); // vai desenhar em
    int oc = rand() % (info->c - 3); // (ol,oc)
    int olh = 3 + rand() % (info->l - ol - 3); // altura desse
    int olw = 3 + rand() % (info->c - oc - 3); // largura desse
    // prepara a cor
    text_color(info->cor, _preto_);
    // prepara o box
    memset(topbar, line[1], olw); // prepara a barra inferior
    topbar[olw] = 0;
    topbar[0] = line[5];
    topbar[olw - 1] = line[4];
    memset(intbar, ' ', olw); // prepara a barra interna
    intbar[olw] = 0;
    intbar[0] = line[3];
    intbar[olw - 1] = line[3];
    int boxL = info->baseL + ol;
    int boxC = info->baseC + oc;
    // log box position
    gotoYX(boxL, boxC);
    printf("%s", topbar); // acima
    gotoYX(boxL + olh - 1, boxC);
    topbar[0] = line[0];
    topbar[olw - 1] = line[2];
    printf("%s", topbar); // abaixo
    for (int b = 1; b < (olh - 1); b += 1)
    {
        gotoYX(boxL + b, boxC);
        printf("%s", intbar);
    };  // for()
    return 0;
};  // box()


void        mostra(_cs_status* antes)
{
    // mostra a fonte original
    wprintf(L"\nFonte original:\n\tFaceName: %ls\n", antes->font_infoEx.FaceName);
    printf(
        "\tFamily: %d\n\tWeight: %d\n\tSize: %dx%d\n\tNumber: %d\n",
        antes->font_infoEx.FontFamily,
        antes->font_infoEx.FontWeight,
        antes->font_infoEx.dwFontSize.X,
        antes->font_infoEx.dwFontSize.Y,
        antes->font_infoEx.nFont
    );
    _cs_infoEx cs_info;
    PCONSOLE_SCREEN_BUFFER_INFOEX info = &(cs_info.infoEx);
    HANDLE  h = GetStdHandle(STD_OUTPUT_HANDLE);
    info->cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    int res = GetConsoleScreenBufferInfoEx(h, info);
    if (res == 0)
    {
        printf("GetConsoleScreenBufferInfoEx() Falhou. Last Error %d\n", GetLastError());
        exit(-1);
    };  // if()
    printf("\nLocale e CodePages:\n");
    printf("\tLocale: '%s'\n\tANSI CP : '%d'\n\t\
Console CP: '%d'\n\tConsole Output CP: '%d'\n\n",
setlocale(LC_ALL, NULL), GetACP(),
GetConsoleCP(), GetConsoleOutputCP()
);
    printf("Console:\n\tdwSize: %d %d\n", info->dwSize.X, info->dwSize.Y);
    printf("\tJanela: (%d linhas x %d colunas)\n\n",
        (info->srWindow.Right - info->srWindow.Left + 1),
        (info->srWindow.Bottom - info->srWindow.Top + 1)
    );
    return;
};  // mostra()


int         prepara_tela(_cs_infoEx* csinfo)
{
    HANDLE  h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX          fonte;
    CONSOLE_SCREEN_BUFFER_INFOEX* scrbInfoEx = &csinfo->infoEx;
    COORD                        xy;
    int                          res = 0;
    srand(20200505); // start rand()
    // confirma tamanho da tela
    scrbInfoEx->cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    res = GetConsoleScreenBufferInfoEx(h, scrbInfoEx);
    if (res == 0) return GetLastError();
    csinfo->l = (scrbInfoEx->srWindow.Bottom - scrbInfoEx->srWindow.Top + 1);
    csinfo->c = (scrbInfoEx->srWindow.Right - scrbInfoEx->srWindow.Left + 1);
    if ((csinfo->l < 24) || (csinfo->c < 80))
    {
        printf("Use uma tela com ao menos 24x80 caracteres\n");
        return -1; // pelo menos 24x80
    };  // if()
    char barra[256];
    memset(barra, 196, 256);
    SetConsoleOutputCP(437);
    SetConsoleCP(437);
    csinfo->baseL = csinfo->baseC = 1;
    csinfo->cor = _verde_;
    csinfo->estilo = 0;
    csinfo->i_handle = GetStdHandle(STD_INPUT_HANDLE);
    csinfo->o_handle = h;
    // define o modo da console
    int   modo = 0;
    GetConsoleMode(h, &modo);
    // DISABLE_NEWLINE_AUTO_RETURN 0x0008 
    // ENABLE_WRAP_AT_EOL_OUTPUT   0x0002
    modo = modo & 0xFFF5; //1111 0101

    // salvo o padrao em uso, muda para o estado que queremos

    SetConsoleMode(h, modo);
    // muda a fonte para Courier New
    fonte.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    fonte.nFont = 0;
    fonte.dwFontSize.X = 10;
    fonte.dwFontSize.Y = 20;
    lstrcpyW(fonte.FaceName, L"Courier New");
    fonte.FontFamily = 54;
    fonte.FontWeight = 400;
    res = SetCurrentConsoleFontEx(h, 0, &fonte);
    if (res == 0)
    {
        printf("SetCurrentConsoleFontEx() falhou: %d\n", GetLastError());
        return (-1);
    };  // if()
    // agora retira o scroll
    xy.Y = scrbInfoEx->srWindow.Bottom - scrbInfoEx->srWindow.Top + 1;
    xy.X = scrbInfoEx->srWindow.Right - scrbInfoEx->srWindow.Left + 1;
    res = SetConsoleScreenBufferSize(h, xy);
    if (res == 0)
    {
        res = GetLastError();
        printf("SetConsoleScreenBufferSize() falhou! Erro : %d \n",
            res);
        return (-1);
    };  // if()
    cls();
    text_color(_verde_, _preto_);
    barra[csinfo->c - 2] = 0;
    gotoYX(csinfo->l - 3, 1);
    printf("%s", barra);
    csinfo->l -= 4;
    csinfo->c -= 2;
    csinfo->baseL = 1; // origem da janela grafica
    csinfo->baseC = 1;
    return 0;
};  // prepara_tela()

void        testa()
{
    // esse e um trecho de um programa de um aluno
    // esta aqui so para testar os acentos
    printf("\n***** teste *****\n");;
    printf("=-= =-= =-= OPERAÇÕES =-= =-= =-= \n");
    printf("[ 1 ] - Adição \n");
    printf("[ 2 ] - Multiplicação \n");
    printf("[ 3 ] - Subtração \n");
    printf("[ 4 ] - Divisão \n");
    printf("\n***** fim do teste *****\n\n\n");;
};  // testa()
