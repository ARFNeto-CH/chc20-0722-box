#pragma once

#define			_CRT_SECURE_NO_WARNINGS
#define         WINVER 0x0A00
#define         _WIN32_WINNT _WIN32_WINNT_WIN10

// define para as cores para simplificar
#define		_preto_            0
#define		_azul_             1
#define		_verde_            2
#define		_ciano_            3
#define		_vermelho_         4
#define		_magenta_          5
#define		_marron_           6
#define		_cinza_claro_      7
#define		_cinza_escuro_     8
#define		_azul_claro_       9
#define		_verde_claro_     10
#define		_ciano_claro_     11
#define		_vermelho_claro_  12
#define		_magenta_claro_   13
#define		_amarelo_         14
#define		_branco_          15

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef struct
{
	int     l; // dimensao em uso da tela
	int     c; // LxC def em prepara_tela()
	int     baseL; // origem na tela
	int		baseC; // (L,C)
	int		bufferL; // tamanho
	int		bufferC; // screen buffer default 9100
	int     cor;
	int     estilo;
	HANDLE  i_handle;
	HANDLE  o_handle;
	FILE* log;
	CONSOLE_SCREEN_BUFFER_INFOEX infoEx;
}   _cs_infoEx;

typedef struct
{
	int     cp_original;
	int     ocp_original;
	char    local_original[40];
	int		mode_original;
	FILE* log;
	CONSOLE_FONT_INFO font_info;
	CONSOLE_FONT_INFOEX font_infoEx;
	CONSOLE_SCREEN_BUFFER_INFO info;
	CONSOLE_SCREEN_BUFFER_INFOEX infoEx;
}  _cs_status;

char		choice();
int			cls();
void		flush();
int	        GetScreenSize(_cs_infoEx*);
void		gotoYX(int, int);
_cs_status* guarda(_cs_status*);
void		mensagem_em_video_reverso(char* mensagem);
void        mostra_grade_256_cores();
void        restaura(_cs_status*);
void		text_color(int, int);

