#pragma once
#include "chConsoleFunctions.h"

int		cls()
{	// limpa a tela no windows, do jeito oficial
	CONSOLE_SCREEN_BUFFER_INFO		info;
	HANDLE		H = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD		origem = { 0,0 };
	int			total;
	if (H == INVALID_HANDLE_VALUE) return -1;
	GetConsoleScreenBufferInfo(H, &info);
	int r = FillConsoleOutputCharacter(H, (TCHAR)' ',
		info.dwSize.X * info.dwSize.Y,
		origem, &total);
	int s = FillConsoleOutputAttribute(
		H, info.wAttributes,
		info.dwSize.X * info.dwSize.Y,
		origem, &total);
	SetConsoleCursorPosition(H, origem);
	return 0;
};	// end cls()


char	choice()
{
	HANDLE	I = GetStdHandle(STD_INPUT_HANDLE);
	int		total = 0;
	typedef struct _INPUT_RECORD Input;
	char	c;
	Input	buffer[32];
	int n = PeekConsoleInput(I, buffer, 32, (LPDWORD)&total);
	if (total == 0) return 0;
	for (int i = 0; i < total; i += 1)
	{
		if (buffer[i].EventType == KEY_EVENT)
			if (buffer[i].Event.KeyEvent.bKeyDown)
				if (c = buffer[i].Event.KeyEvent.uChar.AsciiChar)
				{
					FlushConsoleInputBuffer(I);
					return c;
				};
	};	// if()
	FlushConsoleInputBuffer(I);
	return 0;
};	// choice()


void    flush()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
};  // flush()


void	gotoYX(int linha, int coluna)
{
	static COORD	coord;
	HANDLE			H = GetStdHandle(STD_OUTPUT_HANDLE);
	coord.X = coluna; coord.Y = linha;
	SetConsoleCursorPosition(H, coord);
	return;
};	// gotoXY


_cs_status*    guarda(_cs_status* antes)
{
	antes->cp_original = GetConsoleCP();
	antes->ocp_original = GetConsoleOutputCP();
	strcpy(antes->local_original, setlocale(LC_ALL, NULL));
	HANDLE  h = GetStdHandle(STD_OUTPUT_HANDLE);
	char buffer[80];
	char* pBuffer = buffer;
	int res = 0;
	res = GetCurrentConsoleFont(h, 0, &(antes->font_info));
	// pois e: mas qual a fonte? Tem que ver na tabela
	// precisa da versao ex da rotina e eu nao sabia
	antes->font_infoEx.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	res = GetCurrentConsoleFontEx(h, 0, &(antes->font_infoEx));
	// A fonte em uso fica aqui no registro
	//    Computador\HKEY_LOCAL_MACHINE\SOFTWARE\
    //    Microsoft\Windows NT\CurrentVersion\Console\TrueTypeFont
	//
	GetConsoleMode(h, &antes->mode_original);
	// agora falta SCREEN BUFFER
	antes->infoEx.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(h, &antes->infoEx);
	return antes;
};  // guarda()


void	mensagem_em_video_reverso(char* mensagem)
{
	HANDLE	H = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO		info;
	GetConsoleScreenBufferInfo(H, &info);
	WORD foreground = info.wAttributes & 0xF;
	WORD background = info.wAttributes & 0xF0;
	printf("\n");
	text_color(background, foreground);
	printf("%s\n", mensagem);
	text_color(foreground, background);
	return;
}	// mensagem_em_video_reverso()


void    mostra_grade_256_cores()
{
	cls();
	text_color(15, 0);	/* preto sobre branco */
	printf("Gabarito (Frente|Fundo) escritona linha 5 coluna 10\n");
	printf("    os numeros sao das cores que o windows usa\n\n\n");

	for (int letra = 0; letra < 16; letra += 1)
	{
		for (int fundo = 0; fundo < 16; fundo += 1)
		{
			text_color(letra, fundo);
			printf(" %2d %2d ", letra, fundo);
			text_color(_branco_, _preto_);
			printf("  ");
		}
		text_color(_branco_, _preto_);
		printf(" \n");
	}

	printf("\nteste: antes de escrever em amarelo sobre preto\n");
	text_color(_amarelo_, _preto_);
	printf("Amarelo sobre preto: os numeros sao das cores que o windows usa\n");
	text_color(_branco_, _preto_);
	printf("teste depois de restaurar a cor\n");

	return;
};	// mostra_grade_256_cores()


void    restaura(_cs_status* antes)
{
	// restaura locale e paginas de codigo
	SetConsoleCP(antes->cp_original);
	SetConsoleOutputCP(antes->ocp_original);
	setlocale(LC_ALL, antes->local_original);
	// restaura a fonte
	HANDLE ho = GetStdHandle(STD_OUTPUT_HANDLE);
	int res = SetCurrentConsoleFontEx(ho, 0, &antes->font_infoEx);
	if (res == 0)
		printf("SetCurrentConsoleFontEx() falhou: %d\n", GetLastError());
	// console mode
	SetConsoleMode(ho, antes->mode_original);
	// cor e fundo
	SetConsoleTextAttribute(ho, antes->infoEx.wAttributes);
};  // restaura();


void	text_color(int letras, int fundo)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), letras | (fundo << 4));
}	// text_color

// fim de ch-console.c