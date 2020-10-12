/****************************************************************************
 * [S]imulated [M]edieval [A]dventure multi[U]ser [G]ame      |   \\._.//   *
 * -----------------------------------------------------------|   (0...0)   *
 * SMAUG 1.4 (C) 1994, 1995, 1996, 1998  by Derek Snider      |    ).:.(    *
 * -----------------------------------------------------------|    {o o}    *
 * SMAUG code team: Thoric, Altrag, Blodkai, Narn, Haus,      |   / ' ' \   *
 * Scryn, Rennard, Swordbearer, Gorog, Grishnakh, Nivek,      |~'~.VxvxV.~'~*
 * Tricops and Fireblade                                      |             *
 * ------------------------------------------------------------------------ *
 *			     Special clan module			    *
 ****************************************************************************/

#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "mud.h"



#define MAX_NEST	100
static	OBJ_DATA *	rgObjNest	[MAX_NEST];

CLAN_DATA * first_clan;
CLAN_DATA * last_clan;
COUNCIL_DATA * first_council;
COUNCIL_DATA * last_council;

/* local routines */
void	fread_clan	args( ( CLAN_DATA *clan, FILE *fp ) );
void    busca_con_pj    args( ( CLAN_DATA *familia ));
void    busca_con_area  args( ( CLAN_DATA *familia ) );
bool	load_clan_file	args( ( char *clanfile ) );
void	write_clan_list	args( ( void ) );
void	fread_council	args( ( COUNCIL_DATA *council, FILE *fp ) );
void           save_farea                  args( ( void ) );
bool	load_council_file	args( ( char *councilfile ) );
void	write_council_list	args( ( void ) );
int a_con;
int p_con;
/*
 * Get pointer to clan structure from clan name.
 */
CLAN_DATA *get_clan( char *name )
{
    CLAN_DATA *clan;

    for ( clan = first_clan; clan; clan = clan->next )
       if ( !str_prefix( name, clan->name ) )
         return clan;
    return NULL;
}

COUNCIL_DATA *get_council( char *name )
{
    COUNCIL_DATA *council;

    for ( council = first_council; council; council = council->next )
       if ( !str_cmp( name, council->name ) )
         return council;
    return NULL;
}

void write_clan_list( )
{
    CLAN_DATA *tclan;
    FILE *fpout;
    char filename[256];

    sprintf( filename, "%s%s", CLAN_DIR, CLAN_LIST );
    fpout = fopen( filename, "w" );
    if ( !fpout )
    {
	bug( "FATAL: cannot open clan.lst for writing!\n\r", 0 );
 	return;
    }
    for ( tclan = first_clan; tclan; tclan = tclan->next )
	fprintf( fpout, "%s\n", tclan->filename );
    fprintf( fpout, "$\n" );
    fclose( fpout );
}

void write_council_list( )
{
    COUNCIL_DATA *tcouncil;
    FILE *fpout;
    char filename[256];

    sprintf( filename, "%s%s", COUNCIL_DIR, COUNCIL_LIST );
    fpout = fopen( filename, "w" );
    if ( !fpout )
    {
	bug( "FATAL: cannot open council.lst for writing!\n\r", 0 );
 	return;
    }
    for ( tcouncil = first_council; tcouncil; tcouncil = tcouncil->next )
	fprintf( fpout, "%s\n", tcouncil->filename );
    fprintf( fpout, "$\n" );
    fclose( fpout );
}

/*
 * Save a clan's data to its data file
 */
void save_clan( CLAN_DATA *clan )
{
    FILE *fp;
    AREA_DATA *tarea;
    char filename[256];
    char buf[MAX_STRING_LENGTH];
    int cont = 0;

    if ( !clan )
    {
	bug( "save_clan: null clan pointer!", 0 );
	return;
    }

    if ( !clan->filename || clan->filename[0] == '\0' )
    {
	sprintf( buf, "save_clan: %s has no filename", clan->name );
	bug( buf, 0 );
	return;
    }

    sprintf( filename, "%s%s", CLAN_DIR, clan->filename );

    fclose( fpReserve );
    if ( ( fp = fopen( filename, "w" ) ) == NULL )
    {
    	bug( "save_clan: fopen", 0 );
    	perror( filename );
    }
    else
    {
	fprintf( fp, "#CLAN\n" );
	fprintf( fp, "Name         %s~\n",	clan->name		);
                 fprintf( fp, "Numero           %d\n", clan->numero                             );
	fprintf( fp, "Filename     %s~\n",	clan->filename		);
	fprintf( fp, "Motto        %s~\n",	clan->motto		);
	fprintf( fp, "Description  %s~\n",	clan->description	);
	fprintf( fp, "Deity        %s~\n",	clan->deity		);
	fprintf( fp, "Leader       %s~\n",	clan->leader		);
	fprintf( fp, "NumberOne    %s~\n",	clan->number1		);
	fprintf( fp, "NumberTwo    %s~\n",	clan->number2		);
	fprintf( fp, "Badge        %s~\n",	clan->badge		);
	fprintf( fp, "Leadrank     %s~\n",	clan->leadrank		);
	fprintf( fp, "Onerank      %s~\n",	clan->onerank		);
	fprintf( fp, "Tworank      %s~\n",	clan->tworank		);
	fprintf( fp, "PKillRangeNew   %d %d %d %d %d %d %d\n",
		clan->pkills[0], clan->pkills[1], clan->pkills[2],
		clan->pkills[3], clan->pkills[4], clan->pkills[5],
		clan->pkills[6]);
	fprintf( fp, "PDeathRangeNew  %d %d %d %d %d %d %d\n",
		clan->pdeaths[0], clan->pdeaths[1], clan->pdeaths[2],
		clan->pdeaths[3], clan->pdeaths[4], clan->pdeaths[5],
		clan->pdeaths[6]);
	fprintf( fp, "MKills       %d\n",	clan->mkills		);
	fprintf( fp, "MDeaths      %d\n",	clan->mdeaths		);
	fprintf( fp, "IllegalPK    %d\n",	clan->illegal_pk	);
	fprintf( fp, "Morgue	   %d\n",       clan->morgue		);
	fprintf( fp, "Score        %d\n",	clan->score		);
	fprintf( fp, "Type         %d\n",	clan->clan_type		);
	fprintf( fp, "Class        %d\n",	clan->class		);
	fprintf( fp, "Favour       %d\n",	clan->favour		);
	fprintf( fp, "Strikes      %d\n",	clan->strikes		);
	fprintf( fp, "Members      %d\n",	clan->members		);
	fprintf( fp, "MemLimit     %d\n",	clan->mem_limit		);
	fprintf( fp, "Alignment    %d\n",	clan->alignment		);
	fprintf( fp, "Board        %d\n",	clan->board		);
	fprintf( fp, "ClanObjOne   %d\n",	clan->clanobj1		);
	fprintf( fp, "ClanObjTwo   %d\n",	clan->clanobj2		);
	fprintf( fp, "ClanObjThree %d\n",	clan->clanobj3		);
        fprintf( fp, "ClanObjFour  %d\n",	clan->clanobj4		);
	fprintf( fp, "ClanObjFive  %d\n", 	clan->clanobj5		);
	fprintf( fp, "Recall       %d\n",	clan->recall		);
	fprintf( fp, "Storeroom    %d\n",	clan->storeroom		);
	fprintf( fp, "GuardOne     %d\n",	clan->guard1		);
	fprintf( fp, "GuardTwo     %d\n",	clan->guard2		);
        fprintf( fp, "Puntos       %d\n",       clan->puntos            );
        fprintf( fp, "PCon      %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
 	clan->puntos_conquista[0], clan->puntos_conquista[1], clan->puntos_conquista[2],
         clan->puntos_conquista[3], clan->puntos_conquista[4], clan->puntos_conquista[5],
          clan->puntos_conquista[6], clan->puntos_conquista[7], clan->puntos_conquista[8],
           clan->puntos_conquista[9], clan->puntos_conquista[10], clan->puntos_conquista[11],
            clan->puntos_conquista[12], clan->puntos_conquista[13], clan->puntos_conquista[14],
             clan->puntos_conquista[15], clan->puntos_conquista[16], clan->puntos_conquista[17],
              clan->puntos_conquista[18], clan->puntos_conquista[19], clan->puntos_conquista[20],
               clan->puntos_conquista[21], clan->puntos_conquista[22], clan->puntos_conquista[23],
                clan->puntos_conquista[24], clan->puntos_conquista[25], clan->puntos_conquista[26],
                 clan->puntos_conquista[27], clan->puntos_conquista[28], clan->puntos_conquista[29],
                  clan->puntos_conquista[30], clan->puntos_conquista[31], clan->puntos_conquista[32],
		   clan->puntos_conquista[33] );
         fprintf( fp, "Propietario0      %s~\n",  clan->propietario[0] ? clan->propietario[0] : "Territorrio virgen");
	for ( cont = 1; cont < 32; cont++)
        	fprintf( fp, "Propietario%d      %s~\n", cont, clan->propietario[cont] ? clan->propietario[cont] : "Territorio virgen" );

        fprintf( fp, "Posicion          %d\n",          clan->posicion          );

        fprintf( fp, "Enemigos0          %s~\n", clan->enemigos[0] );
        fprintf( fp, "Enemigos1          %s~\n", clan->enemigos[1] );
        fprintf( fp, "Enemigos2          %s~\n", clan->enemigos[2] );
        fprintf( fp, "Enemigos3          %s~\n", clan->enemigos[3] );
        fprintf( fp, "Enemigos4          %s~\n", clan->enemigos[4] );
        fprintf( fp, "Enemigos5          %s~\n", clan->enemigos[5] );
        fprintf( fp, "Enemigos6          %s~\n", clan->enemigos[6] );
        fprintf( fp, "Enemigos7          %s~\n", clan->enemigos[7] );
        fprintf( fp, "Enemigos8          %s~\n", clan->enemigos[8] );
        fprintf( fp, "Enemigos9          %s~\n", clan->enemigos[9] );

        fprintf( fp, "Tregua            %s~\n",          clan->tregua );
        fprintf( fp, "Odio             %d %d %d %d %d %d %d %d %d %d\n", clan->odio[0], clan->odio[1], clan->odio[2], clan->odio[3], clan->odio[4], clan->odio[5], clan->odio[6], clan->odio[7], clan->odio[8], clan->odio[9] );
        fprintf( fp, "Alianza           %d %d %d %d %d %d %d %d %d %d\n", clan->alianza[0], clan->alianza[1], clan->alianza[2], clan->alianza[3], clan->alianza[4], clan->alianza[5], clan->alianza[6], clan->alianza[7], clan->alianza[8], clan->alianza[9] );

        fprintf( fp, "ProAlianza0        %s~\n", clan->proalianza[0] );
        fprintf( fp, "ProAlianza1        %s~\n", clan->proalianza[1] );
        fprintf( fp, "ProAlianza2        %s~\n", clan->proalianza[2] );
        fprintf( fp, "ProAlianza3        %s~\n", clan->proalianza[3] );
        fprintf( fp, "ProAlianza4        %s~\n", clan->proalianza[4] );
        fprintf( fp, "ProAlianza5        %s~\n", clan->proalianza[5] );
        fprintf( fp, "ProAlianza6        %s~\n", clan->proalianza[6] );
        fprintf( fp, "ProAlianza7        %s~\n", clan->proalianza[7] );
        fprintf( fp, "ProAlianza8        %s~\n", clan->proalianza[8] );
        fprintf( fp, "ProAlianza9        %s~\n", clan->proalianza[9] );

        fprintf( fp, "NivAlianza        %d %d %d %d %d % d %d %d %d %d\n", clan->nv_alianza[0], clan->nv_alianza[1], clan->nv_alianza[2], clan->nv_alianza[3], clan->nv_alianza[4],  clan->nv_alianza[5], clan->nv_alianza[6], clan->nv_alianza[7], clan->nv_alianza[8], clan->nv_alianza[9] );
        fprintf( fp, "NivGuerra         %d %d %d %d %d % d %d %d %d %d\n", clan->nv_guerra[0], clan->nv_guerra[1], clan->nv_guerra[2], clan->nv_guerra[3], clan->nv_guerra[4],  clan->nv_guerra[5], clan->nv_guerra[6], clan->nv_guerra[7], clan->nv_guerra[8], clan->nv_guerra[9] );
        fprintf( fp, "Conquistas    %d\n", clan->conquistas );
        fprintf( fp, "Muerte_guerra     %d %d %d %d %d % d %d %d %d %d\n", clan->muerte_guerra[0], clan->muerte_guerra[1], clan->muerte_guerra[2], clan->muerte_guerra[3], clan->muerte_guerra[4], clan->muerte_guerra[5], clan->muerte_guerra[6], clan->muerte_guerra[7], clan->muerte_guerra[8], clan->muerte_guerra[9] );
        fprintf( fp, "Victorias_guerra         %d %d %d %d %d % d %d %d %d %d\n", clan->victorias_guerra[0], clan->victorias_guerra[1], clan->victorias_guerra[2], clan->victorias_guerra[3], clan->victorias_guerra[4], clan->victorias_guerra[5], clan->victorias_guerra[6], clan->victorias_guerra[7], clan->victorias_guerra[8], clan->victorias_guerra[9] );
        fprintf( fp, "Tierras           %d\n", clan->tierras );
        fprintf( fp, "Victorias       %d\n", clan->victorias );
        fprintf( fp, "Derrotas        %d\n", clan->derrotas );
        fprintf( fp, "Agresor           %d %d %d %d %d % d %d %d %d %d\n", clan->agresor[0], clan->agresor[1], clan->agresor[2], clan->agresor[3], clan->agresor[4],  clan->agresor[5], clan->agresor[6], clan->agresor[7], clan->agresor[8], clan->agresor[9] );
        fprintf( fp, "Oro       %d\n", clan->oro );
        fprintf( fp, "Mineral   %d\n", clan->mineral );
        fprintf( fp, "Madera    %d\n", clan->madera );
        fprintf( fp, "Viveres   %d\n", clan->viveres );
        fprintf( fp, "Agua      %d\n", clan->agua );
        fprintf( fp, "Combustible %d\n", clan->combustibles );
        fprintf( fp, "Hashis    %d      %d\n", clan->drogas[0], clan->calidad[0] );
        fprintf( fp, "Marihuana %d      %d\n", clan->drogas[1], clan->calidad[1] );
        fprintf( fp, "MDMA      %d      %d\n", clan->drogas[2], clan->calidad[2] );
        fprintf( fp, "Hongos    %d      %d\n", clan->drogas[3], clan->calidad[3] );
        fprintf( fp, "Mescalinidos    %d      %d\n", clan->drogas[4], clan->calidad[4] );
        fprintf( fp, "Coca      %d      %d\n", clan->drogas[5], clan->calidad[5] );
        fprintf( fp, "Experiencia %d\n", clan->experiencia );
        fprintf( fp, "Sequito   %d\n", clan->seguidores );
        fprintf( fp, "Estatus   %s~\n", clan->estatus_social );
        fprintf( fp, "Deudas    %d\n", clan->deudas );
        fprintf( fp, "Influencia %d %d %d %d\n",clan->influencia[0], clan->influencia[1], clan->influencia[2], clan->influencia[3] );
    	fprintf( fp, "\nEnd\n\n"						);
	fprintf( fp, "#END\n"						);
    }
    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

/*
 * Save a council's data to its data file
 */
void save_council( COUNCIL_DATA *council )
{
    FILE *fp;
    char filename[256];
    char buf[MAX_STRING_LENGTH];

    if ( !council )
    {
	bug( "save_council: null council pointer!", 0 );
	return;
    }

    if ( !council->filename || council->filename[0] == '\0' )
    {
	sprintf( buf, "save_council: %s has no filename", council->name );
	bug( buf, 0 );
	return;
    }

    sprintf( filename, "%s%s", COUNCIL_DIR, council->filename );

    fclose( fpReserve );
    if ( ( fp = fopen( filename, "w" ) ) == NULL )
    {
    	bug( "save_council: fopen", 0 );
    	perror( filename );
    }
    else
    {
	fprintf( fp, "#COUNCIL\n" );
	fprintf( fp, "Name         %s~\n",	council->name		);
	fprintf( fp, "Filename     %s~\n",	council->filename	);
	fprintf( fp, "Description  %s~\n",	council->description	);
	fprintf( fp, "Head         %s~\n",	council->head		);
      	if ( council->head2 != NULL)
        	fprintf (fp, "Head2        %s~\n", council->head2);
	fprintf( fp, "Members      %d\n",	council->members	);
	fprintf( fp, "Board        %d\n",	council->board		);
	fprintf( fp, "Meeting      %d\n",	council->meeting	);
	fprintf( fp, "Powers       %s~\n",	council->powers		);
	fprintf( fp, "End\n\n"						);
	fprintf( fp, "#END\n"						);
    }
    fclose( fp );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}


/*
 * Read in actual clan data.
 */

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )                                    \
                                if ( !str_cmp( word, literal ) )        \
                                {                                       \
                                    field  = value;                     \
                                    fMatch = TRUE;                      \
                                    break;                              \
                                }

/*
 * Reads in PKill and PDeath still for backward compatibility but now it
 * should be written to PKillRange and PDeathRange for multiple level pkill
 * tracking support. --Shaddai
 * Added a hardcoded limit memlimit to the amount of members a clan can
 * have set using setclan.  --Shaddai
 */

void fread_clan( CLAN_DATA *clan, FILE *fp )
{
    char buf[MAX_STRING_LENGTH];
    char *word;
    bool fMatch;
    int cnt;


    clan->mem_limit = 0;  /* Set up defaults */
    for ( ; ; )
    {
	word   = feof( fp ) ? "End" : fread_word( fp );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'A':
                     if( !str_cmp( word, "Agresor" ) )
                     {
                                          fMatch = TRUE;
                                clan->agresor[0] = fread_number( fp );
                                clan->agresor[1] = fread_number( fp );
                                clan->agresor[2] = fread_number( fp );
                                clan->agresor[3] = fread_number( fp );
                                clan->agresor[4] = fread_number( fp );
                                clan->agresor[5] = fread_number( fp );
                                clan->agresor[6] = fread_number( fp );
                                clan->agresor[7] = fread_number( fp );
                                clan->agresor[8] = fread_number( fp );
                                clan->agresor[9] = fread_number( fp );
                     }

                     if( !str_cmp( word, "Alianza" ) )
                     {
                                          fMatch = TRUE;
                                clan->alianza[0] = fread_number( fp );
                                clan->alianza[1] = fread_number( fp );
                                clan->alianza[2] = fread_number( fp );
                                clan->alianza[3] = fread_number( fp );
                                clan->alianza[4] = fread_number( fp );
                                clan->alianza[5] = fread_number( fp );
                                clan->alianza[6] = fread_number( fp );
                                clan->alianza[7] = fread_number( fp );
                                clan->alianza[8] = fread_number( fp );
                                clan->alianza[9] = fread_number( fp );
                     }
                     KEY( "Alignment",	clan->alignment,	fread_number( fp ) );
                     KEY( "Agua",       clan->agua,             fread_number( fp ) );
	    break;

	case 'B':
            KEY( "Badge",       clan->badge,            fread_string( fp ) );
	    KEY( "Board",	clan->board,		fread_number( fp ) );
	    break;

	case 'C':
                     if ( !str_cmp( word, "Castigo" ) )
                     {
                                fMatch =  TRUE;
                         clan->castigos[0] = fread_number( fp );
                         clan->castigos[1] = fread_number( fp );
                         clan->castigos[2] = fread_number( fp );
                         clan->castigos[3] = fread_number( fp );
                         clan->castigos[4] = fread_number( fp );
                         clan->castigos[5] = fread_number( fp );
                    }
	    KEY( "ClanObjOne",	clan->clanobj1,		fread_number( fp ) );
	    KEY( "ClanObjTwo",	clan->clanobj2,		fread_number( fp ) );
	    KEY( "ClanObjThree",clan->clanobj3,		fread_number( fp ) );
            KEY( "ClanObjFour", clan->clanobj4,         fread_number( fp ) );
            KEY( "ClanObjFive", clan->clanobj5,         fread_number( fp ) );
	    KEY( "Class",	clan->class,		fread_number( fp ) );
            KEY( "Coca",        clan->drogas[5],        fread_number( fp ) );
            KEY( "Conquistas", clan->conquistas,        fread_number( fp ) );
            KEY( "Combustible", clan->combustibles,     fread_number( fp ) );
	    break;

	case 'D':
	    KEY( "Deity",	clan->deity,		fread_string( fp ) );
            KEY( "Derrotas",    clan->derrotas,         fread_number( fp ) );
	    KEY( "Description",	clan->description,	fread_string( fp ) );
            KEY( "Deudas",      clan->deudas,           fread_number( fp ) );
	    break;

	case 'E':
                  KEY( "Enemigos0", clan->enemigos[0],          fread_string( fp ) );
                  KEY( "Enemigos1", clan->enemigos[1],          fread_string( fp ) );
                  KEY( "Enemigos2", clan->enemigos[2],          fread_string( fp ) );
                  KEY( "Enemigos3", clan->enemigos[3],          fread_string( fp ) );
                  KEY( "Enemigos4", clan->enemigos[4],          fread_string( fp ) );
                  KEY( "Enemigos5", clan->enemigos[5],          fread_string( fp ) );
                  KEY( "Enemigos6", clan->enemigos[6],          fread_string( fp ) );
                  KEY( "Enemigos7", clan->enemigos[7],          fread_string( fp ) );
                  KEY( "Enemigos8", clan->enemigos[8],          fread_string( fp ) );
                  KEY( "Enemigos9", clan->enemigos[9],          fread_string( fp ) );
                 if ( !str_cmp( word, "End" ) )
	    {
		if (!clan->name)
		  clan->name		= STRALLOC( "" );
		if (!clan->leader)
		  clan->leader		= STRALLOC( "" );
		if (!clan->description)
		  clan->description 	= STRALLOC( "" );
		if (!clan->motto)
		  clan->motto		= STRALLOC( "" );
		if (!clan->number1)
		  clan->number1		= STRALLOC( "" );
		if (!clan->number2)
		  clan->number2		= STRALLOC( "" );
		if (!clan->deity)
		  clan->deity		= STRALLOC( "" );
		if (!clan->badge)
	  	  clan->badge		= STRALLOC( "" );
		if (!clan->leadrank)
		  clan->leadrank	= STRALLOC( "" );
		if (!clan->onerank)
		  clan->onerank		= STRALLOC( "" );
		if (!clan->tworank)
		  clan->tworank		= STRALLOC( "" );
		return;
	    }
                KEY( "Estatus",         clan->estatus_social,    fread_string( fp ) );
                KEY( "Experiencia",     clan->experiencia,      fread_number( fp ) );
	    break;

	case 'F':
	    KEY( "Favour",	clan->favour,		fread_number( fp ) );
	    KEY( "Filename",	clan->filename,		fread_string_nohash( fp ) );

	case 'G':
	    KEY( "GuardOne",	clan->guard1,		fread_number( fp ) );
	    KEY( "GuardTwo",	clan->guard2,		fread_number( fp ) );
	    break;

        case 'H':
            KEY( "Hashis",      clan->drogas[0],         fread_number( fp ) );
            KEY( "Hongos",      clan->drogas[3],         fread_number( fp ) );
            break;

	case 'I':
	    KEY( "IllegalPK",	clan->illegal_pk,	fread_number( fp ) );
                      if ( !str_cmp( word, "Impuestos" ) )
                      {
                                fMatch = TRUE;
                                clan->impuestos[0] = fread_number( fp );
                                clan->impuestos[1] = fread_number( fp );
                       }

                      if ( !str_prefix( word, "Influencia" ) )
                      {
                                fMatch = TRUE;
                                clan->influencia[0] = fread_number( fp );
                                clan->influencia[1] = fread_number( fp );
                                clan->influencia[2] = fread_number( fp );
                                clan->influencia[3] = fread_number( fp );
                      }
	    break;

	case 'L':
	    KEY( "Leader",	clan->leader,		fread_string( fp ) );
	    KEY( "Leadrank",	clan->leadrank,		fread_string( fp ) );
	    break;

	case 'M':
            KEY( "Marihuana",   clan->drogas[1],         fread_number( fp ) );
            KEY( "MDMA",        clan->drogas[2],         fread_number( fp ) );
            KEY( "Madera",      clan->madera,           fread_number( fp ) );
            KEY( "MDeaths",	clan->mdeaths,		fread_number( fp ) );
	    KEY( "Members",	clan->members,		fread_number( fp ) );
	    KEY( "MemLimit",	clan->mem_limit,	fread_number( fp ) );
            KEY( "Mescalinidos", clan->drogas[4],       fread_number( fp ) );
            KEY( "Mineral",     clan->mineral,          fread_number( fp ) );
	    KEY( "MKills",	clan->mkills,		fread_number( fp ) );
	    KEY( "Motto",	clan->motto,		fread_string( fp ) );
	    KEY( "Morgue",	clan->morgue,		fread_number( fp ) );
                     if( !str_cmp( word, "Muerte_guerra") )
                     {
                                fMatch = TRUE;
                                clan->muerte_guerra[0] =  fread_number( fp ) ;
                                clan->muerte_guerra[1] =  fread_number( fp ) ;
                                clan->muerte_guerra[2] =  fread_number( fp ) ;
                                clan->muerte_guerra[3] =  fread_number( fp ) ;
                                clan->muerte_guerra[4] =  fread_number( fp ) ;
                                clan->muerte_guerra[5] =  fread_number( fp ) ;
                                clan->muerte_guerra[6] =  fread_number( fp ) ;
                                clan->muerte_guerra[7] =  fread_number( fp ) ;
                                clan->muerte_guerra[8] =  fread_number( fp ) ;
                                clan->muerte_guerra[9] =  fread_number( fp ) ;
                     }


	case 'N':
	    KEY( "Name",	clan->name,		fread_string( fp ) );
	    KEY( "NumberOne",	clan->number1,		fread_string( fp ) );
	    KEY( "NumberTwo",	clan->number2,		fread_string( fp ) );
                      KEY( "Numero",                    clan->numero,                   fread_number( fp ) );
                     if( !str_cmp( word, "NivAlianza") )
                     {
                                fMatch = TRUE;
                                clan->nv_alianza[0] =  fread_number( fp ) ;
                                clan->nv_alianza[1] =  fread_number( fp ) ;
                                clan->nv_alianza[2] =  fread_number( fp ) ;
                                clan->nv_alianza[3] =  fread_number( fp ) ;
                                clan->nv_alianza[4] =  fread_number( fp ) ;
                                clan->nv_alianza[5] =  fread_number( fp ) ;
                                clan->nv_alianza[6] =  fread_number( fp ) ;
                                clan->nv_alianza[7] =  fread_number( fp ) ;
                                clan->nv_alianza[8] =  fread_number( fp ) ;
                                clan->nv_alianza[9] =  fread_number( fp ) ;
                     }
                    if( !str_cmp( word, "NivGuerra" ) )
                    {
                               fMatch = TRUE;
                                clan->nv_guerra[0] = fread_number( fp );
                                clan->nv_guerra[1] = fread_number( fp );
                                clan->nv_guerra[2] = fread_number( fp );
                                clan->nv_guerra[3] = fread_number( fp );
                                clan->nv_guerra[4] = fread_number( fp );
                                clan->nv_guerra[5] = fread_number( fp );
                                clan->nv_guerra[6] = fread_number( fp );
                                clan->nv_guerra[7] = fread_number( fp );
                                clan->nv_guerra[8] = fread_number( fp );
                                clan->nv_guerra[9] = fread_number( fp );
                      }
	    break;

	case 'O':
                     if( !str_cmp( word, "Odio" ) )
                     {
                               fMatch = TRUE;
                                clan->odio[0] = fread_number( fp );
                                clan->odio[1] = fread_number( fp );
                                clan->odio[2] = fread_number( fp );
                                clan->odio[3] = fread_number( fp );
                                clan->odio[4] = fread_number( fp );
                                clan->odio[5] = fread_number( fp );
                                clan->odio[6] = fread_number( fp );
                                clan->odio[7] = fread_number( fp );
                                clan->odio[8] = fread_number( fp );
                                clan->odio[9] = fread_number( fp );
                       }
	    KEY( "Onerank",	clan->onerank,		fread_string( fp ) );
                     KEY( "Oro",        clan->oro,              fread_number( fp ) );
	    break;

	case 'P':
                      if ( !str_cmp( word, "PCon" ) )
                      {
                                fMatch = TRUE;
                                clan->puntos_conquista[0] = fread_number( fp );
                                clan->puntos_conquista[1] = fread_number( fp );
                                clan->puntos_conquista[2] = fread_number( fp );
                                clan->puntos_conquista[3] = fread_number( fp );
                                clan->puntos_conquista[4] = fread_number( fp );
                                clan->puntos_conquista[5] = fread_number( fp );
                                clan->puntos_conquista[6] = fread_number( fp );
                                clan->puntos_conquista[7] = fread_number( fp );
                                clan->puntos_conquista[8] = fread_number( fp );
                                clan->puntos_conquista[9] = fread_number( fp );
                                clan->puntos_conquista[10] = fread_number( fp );
                                clan->puntos_conquista[11] = fread_number( fp );
                                clan->puntos_conquista[12] = fread_number( fp );
                                clan->puntos_conquista[13] = fread_number( fp );
                                clan->puntos_conquista[14] = fread_number( fp );
                                clan->puntos_conquista[15] = fread_number( fp );
                                clan->puntos_conquista[16] = fread_number( fp );
                                clan->puntos_conquista[17] = fread_number( fp );
                                clan->puntos_conquista[18] = fread_number( fp );
                                clan->puntos_conquista[19] = fread_number( fp );
                                clan->puntos_conquista[20] = fread_number( fp );
                                clan->puntos_conquista[21] = fread_number( fp );
                                clan->puntos_conquista[22] = fread_number( fp );
                                clan->puntos_conquista[23] = fread_number( fp );
                                clan->puntos_conquista[24] = fread_number( fp );
                                clan->puntos_conquista[25] = fread_number( fp );
                                clan->puntos_conquista[26] = fread_number( fp );
                                clan->puntos_conquista[27] = fread_number( fp );
                                clan->puntos_conquista[28] = fread_number( fp );
                                clan->puntos_conquista[29] = fread_number( fp );
                                clan->puntos_conquista[30] = fread_number( fp );
                                clan->puntos_conquista[31] = fread_number( fp );
                                clan->puntos_conquista[32] = fread_number( fp );
                          }
	    KEY( "PDeaths",	clan->pdeaths[6],	fread_number( fp ) );
	    KEY( "PKills",	clan->pkills[6],	fread_number( fp ) );
	    /* Addition of New Ranges */
	    if ( !str_cmp ( word, "PDeathRange" ) )
	    {
		fMatch = TRUE;
		fread_number( fp );
		fread_number( fp );
		fread_number( fp );
		fread_number( fp );
		fread_number( fp );
		fread_number( fp );
		fread_number( fp );
            }
	    if ( !str_cmp ( word, "PDeathRangeNew" ) )
            {
		fMatch = TRUE;
		clan->pdeaths[0] = fread_number( fp );
		clan->pdeaths[1] = fread_number( fp );
		clan->pdeaths[2] = fread_number( fp );
		clan->pdeaths[3] = fread_number( fp );
		clan->pdeaths[4] = fread_number( fp );
		clan->pdeaths[5] = fread_number( fp );
		clan->pdeaths[6] = fread_number( fp );
	    }
	    if ( !str_cmp ( word, "PKillRangeNew" ) )
            {
		fMatch = TRUE;
		clan->pkills[0] = fread_number( fp );
		clan->pkills[1] = fread_number( fp );
		clan->pkills[2] = fread_number( fp );
		clan->pkills[3] = fread_number( fp );
		clan->pkills[4] = fread_number( fp );
		clan->pkills[5] = fread_number( fp );
		clan->pkills[6] = fread_number( fp );
	    }
	    if ( !str_cmp ( word, "PKillRange" ) )
	    {
		fMatch = TRUE;
		fread_number( fp );
		fread_number( fp );
		fread_number( fp );
		fread_number( fp );
		fread_number( fp );
		fread_number( fp );
		fread_number( fp );
            }
                     KEY( "ProAlianza0", clan->proalianza[0], fread_string( fp ) );
                     KEY( "ProAlianza1", clan->proalianza[1], fread_string( fp ) );
                               KEY( "ProAlianza2", clan->proalianza[2], fread_string( fp ));
                                KEY( "ProAlianza3", clan->proalianza[3], fread_string( fp ));
                                KEY( "ProAlianza4", clan->proalianza[4], fread_string( fp ));
                                KEY( "ProAlianza5", clan->proalianza[5], fread_string( fp ));
                                KEY( "ProAlianza6", clan->proalianza[6], fread_string( fp ));
                                KEY( "ProAlianza7", clan->proalianza[7], fread_string( fp ));
                                KEY( "ProAlianza8", clan->proalianza[8], fread_string( fp ));
                                KEY( "ProAlianza9", clan->proalianza[9], fread_string( fp ));

                KEY( "Propietario0",    clan->propietario[0],           fread_string( fp ) );
                KEY( "Propietario1",    clan->propietario[1],           fread_string( fp ) );
                KEY( "Propietario2",    clan->propietario[2],           fread_string( fp ) );
                KEY( "Propietario3",    clan->propietario[3],           fread_string( fp ) );
                KEY( "Propietario4",    clan->propietario[4],           fread_string( fp ) );
                KEY( "Propietario5",    clan->propietario[5],           fread_string( fp ) );
                KEY( "Propietario6",    clan->propietario[6],           fread_string( fp ) );
                KEY( "Propietario7",    clan->propietario[7],           fread_string( fp ) );
                KEY( "Propietario8",    clan->propietario[8],           fread_string( fp ) );
                KEY( "Propietario9",    clan->propietario[9],           fread_string( fp ) );
                KEY( "Propietario10",    clan->propietario[10],           fread_string( fp ) );
                KEY( "Propietario11",    clan->propietario[11],           fread_string( fp ) );
                KEY( "Propietario12",    clan->propietario[12],           fread_string( fp ) );
                KEY( "Propietario13",    clan->propietario[13],           fread_string( fp ) );
                KEY( "Propietario14",    clan->propietario[14],           fread_string( fp ) );
                KEY( "Propietario15",    clan->propietario[15],           fread_string( fp ) );
                KEY( "Propietario16",    clan->propietario[16],           fread_string( fp ) );
                KEY( "Propietario17",    clan->propietario[17],           fread_string( fp ) );
                KEY( "Propietario18",    clan->propietario[18],           fread_string( fp ) );
                KEY( "Propietario19",    clan->propietario[19],           fread_string( fp ) );
                KEY( "Propietario20",    clan->propietario[20],           fread_string( fp ) );
                KEY( "Propietario21",    clan->propietario[21],           fread_string( fp ) );
                KEY( "Propietario22",    clan->propietario[22],           fread_string( fp ) );
                KEY( "Propietario23",    clan->propietario[23],           fread_string( fp ) );
                KEY( "Propietario24",    clan->propietario[24],           fread_string( fp ) );
                KEY( "Propietario25",    clan->propietario[25],           fread_string( fp ) );
                KEY( "Propietario26",    clan->propietario[26],           fread_string( fp ) );
                KEY( "Propietario27",    clan->propietario[27],           fread_string( fp ) );
                KEY( "Propietario28",    clan->propietario[28],           fread_string( fp ) );
                KEY( "Propietario29",    clan->propietario[29],           fread_string( fp ) );
                KEY( "Propietario30",    clan->propietario[30],           fread_string( fp ) );
                KEY( "Propietario31",    clan->propietario[31],           fread_string( fp ) );
                KEY( "Propietario32",    clan->propietario[32],           fread_string( fp ) );


                     KEY( "Posicion",   clan->posicion,         fread_number( fp ) );
                     KEY( "Puntos",     clan->puntos,           fread_number( fp ) );
                     break;


	case 'R':
	    KEY( "Recall",	clan->recall,		fread_number( fp ) );
	    break;

	case 'S':
	    KEY( "Score",	clan->score,		fread_number( fp ) );
            KEY( "Sequito",     clan->seguidores,       fread_number( fp ) );
	    KEY( "Strikes",	clan->strikes,		fread_number( fp ) );
	    KEY( "Storeroom",	clan->storeroom,	fread_number( fp ) );
	    break;

	case 'T':
                    KEY( "Tierras",      clan->tierras,         fread_number( fp ) );
                    KEY( "Tregua",      clan->tregua,           fread_string( fp ) );
                    if ( !str_cmp( word, "Tributos" ) )
                    {
                               fMatch = TRUE;
                        clan->tributo[0] = fread_number( fp );
                        clan->tributo[1] = fread_number( fp );
                        clan->tributo[2] = fread_number( fp );
                        clan->tributo[3] = fread_number( fp );
                        clan->tributo[4] = fread_number( fp );
                        clan->tributo[5] = fread_number( fp );
                    }
	    KEY( "Tworank",	clan->tworank,		fread_string( fp ) );
	    KEY( "Type",	clan->clan_type,	fread_number( fp ) );
	    break;

        case 'V':
        KEY( "Victorias",       clan->victorias,        fread_number( fp ) );
        KEY( "Viveres",         clan->viveres,          fread_number( fp ) );
        if( !str_cmp( word, "Victorias_guerra") )
                     {
                                fMatch = TRUE;
                                clan->victorias_guerra[0] =  fread_number( fp ) ;
                                clan->victorias_guerra[1] =  fread_number( fp ) ;
                                clan->victorias_guerra[2] =  fread_number( fp ) ;
                                clan->victorias_guerra[3] =  fread_number( fp ) ;
                                clan->victorias_guerra[4] =  fread_number( fp ) ;
                                clan->victorias_guerra[5] =  fread_number( fp ) ;
                                clan->victorias_guerra[6] =  fread_number( fp ) ;
                                clan->victorias_guerra[7] =  fread_number( fp ) ;
                                clan->victorias_guerra[8] =  fread_number( fp ) ;
                                clan->victorias_guerra[9] =  fread_number( fp ) ;
                     }

	}

	if ( !fMatch )
	{
	    sprintf( buf, "Fread_clan: no match: %s", word );
	    bug( buf, 0 );
	}
    }
}

/*
 * Read in actual council data.
 */

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
				if ( !str_cmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = TRUE;			\
				    break;				\
				}

void fread_council( COUNCIL_DATA *council, FILE *fp )
{
    char buf[MAX_STRING_LENGTH];
    char *word;
    bool fMatch;

    for ( ; ; )
    {
	word   = feof( fp ) ? "End" : fread_word( fp );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'B':
	    KEY( "Board",	council->board,		fread_number( fp ) );
	    break;

	case 'D':
	    KEY( "Description",	council->description,	fread_string( fp ) );
	    break;

	case 'E':
	    if ( !str_cmp( word, "End" ) )
	    {
		if (!council->name)
		  council->name		= STRALLOC( "" );
		if (!council->description)
		  council->description 	= STRALLOC( "" );
		if (!council->powers)
		  council->powers	= STRALLOC( "" );
		return;
	    }
	    break;

	case 'F':
	    KEY( "Filename",	council->filename,	fread_string_nohash( fp ) );
  	    break;

	case 'H':
	    KEY( "Head", 	council->head, 		fread_string( fp ) );
            KEY ("Head2", 	council->head2, 	fread_string( fp ) );
	    break;

	case 'M':
	    KEY( "Members",	council->members,	fread_number( fp ) );
	    KEY( "Meeting",   	council->meeting, 	fread_number( fp ) );
	    break;

	case 'N':
	    KEY( "Name",	council->name,		fread_string( fp ) );
	    break;

	case 'P':
	    KEY( "Powers",	council->powers,	fread_string( fp ) );
	    break;
	}

	if ( !fMatch )
	{
	    sprintf( buf, "Fread_council: no match: %s", word );
	    bug( buf, 0 );
	}
    }
}


/*
 * Load a clan file
 */

bool load_clan_file( char *clanfile )
{
    char filename[256];
    CLAN_DATA *clan;
    FILE *fp;
    bool found;

    CREATE( clan, CLAN_DATA, 1 );

    /* Make sure we default these to 0 --Shaddai */
    clan->pkills[0] = 0;
    clan->pkills[1] = 0;
    clan->pkills[2] = 0;
    clan->pkills[3] = 0;
    clan->pkills[4] = 0;
    clan->pkills[5] = 0;
    clan->pkills[6] = 0;
    clan->pdeaths[0]= 0;
    clan->pdeaths[1]= 0;
    clan->pdeaths[2]= 0;
    clan->pdeaths[3]= 0;
    clan->pdeaths[4]= 0;
    clan->pdeaths[5]= 0;
    clan->pdeaths[6]= 0;

    found = FALSE;
    sprintf( filename, "%s%s", CLAN_DIR, clanfile );

    if ( ( fp = fopen( filename, "r" ) ) != NULL )
    {

	found = TRUE;
	for ( ; ; )
	{
	    char letter;
	    char *word;

	    letter = fread_letter( fp );
	    if ( letter == '*' )
	    {
		fread_to_eol( fp );
		continue;
	    }

	    if ( letter != '#' )
	    {
		bug( "Load_clan_file: # not found.", 0 );
		break;
	    }

	    word = fread_word( fp );
	    if ( !str_cmp( word, "CLAN"	) )
	    {
	    	fread_clan( clan, fp );
	    	break;
	    }
	    else
	    if ( !str_cmp( word, "END"	) )
	        break;
	    else
	    {
		char buf[MAX_STRING_LENGTH];

		sprintf( buf, "Load_clan_file: bad section: %s.", word );
		bug( buf, 0 );
		break;
	    }
	}
	fclose( fp );
    }

    if ( found )
    {
	ROOM_INDEX_DATA *storeroom;

	LINK( clan, first_clan, last_clan, next, prev );

	if ( clan->storeroom == 0
	|| (storeroom = get_room_index( clan->storeroom )) == NULL )
	{
	    log_string( "Storeroom not found" );
	    return found;
	}

	sprintf( filename, "%s%s.vault", CLAN_DIR, clan->filename );
	if ( ( fp = fopen( filename, "r" ) ) != NULL )
	{
	    int iNest;
	    bool found;
	    OBJ_DATA *tobj, *tobj_next;

	    log_string( "Loading clan storage room" );
	    rset_supermob(storeroom);
	    for ( iNest = 0; iNest < MAX_NEST; iNest++ )
		rgObjNest[iNest] = NULL;

	    found = TRUE;
	    for ( ; ; )
	    {
		char letter;
		char *word;

		letter = fread_letter( fp );
		if ( letter == '*' )
		{
		    fread_to_eol( fp );
		    continue;
		}

		if ( letter != '#' )
		{
		    bug( "Load_clan_vault: # not found.", 0 );
		    bug( clan->name, 0 );
		    break;
		}

		word = fread_word( fp );
		if ( !str_cmp( word, "OBJECT" ) )	/* Objects	*/
		  fread_obj  ( supermob, fp, OS_CARRY );
		else
		if ( !str_cmp( word, "END"    ) )	/* Done		*/
		  break;
		else
		{
		    bug( "Load_clan_vault: bad section.", 0 );
		    bug( clan->name, 0 );
		    break;
		}
	    }
	    fclose( fp );
	    for ( tobj = supermob->first_carrying; tobj; tobj = tobj_next )
	    {
		tobj_next = tobj->next_content;
		obj_from_char( tobj );
		obj_to_room( tobj, storeroom );
	    }
	    release_supermob();
	}
	else
	    log_string( "Cannot open clan vault" );
    }
    else
      DISPOSE( clan );

    return found;
}

/*
 * Load a council file
 */

bool load_council_file( char *councilfile )
{
    char filename[256];
    COUNCIL_DATA *council;
    FILE *fp;
    bool found;

    CREATE( council, COUNCIL_DATA, 1 );

    found = FALSE;
    sprintf( filename, "%s%s", COUNCIL_DIR, councilfile );

    if ( ( fp = fopen( filename, "r" ) ) != NULL )
    {

	found = TRUE;
	for ( ; ; )
	{
	    char letter;
	    char *word;

	    letter = fread_letter( fp );
	    if ( letter == '*' )
	    {
		fread_to_eol( fp );
		continue;
	    }

	    if ( letter != '#' )
	    {
		bug( "Load_council_file: # not found.", 0 );
		break;
	    }

	    word = fread_word( fp );
	    if ( !str_cmp( word, "COUNCIL"	) )
	    {
	    	fread_council( council, fp );
	    	break;
	    }
	    else
	    if ( !str_cmp( word, "END"	) )
	        break;
	    else
	    {
		bug( "Load_council_file: bad section.", 0 );
		break;
	    }
	}
	fclose( fp );
    }

    if ( found )
      LINK( council, first_council, last_council, next, prev );

    else
      DISPOSE( council );

    return found;
}

/*
 * Load in all the clan files.
 */
void load_clans( )
{
    FILE *fpList;
    char *filename;
    char clanlist[256];
    char buf[MAX_STRING_LENGTH];


    first_clan	= NULL;
    last_clan	= NULL;

    log_string( "Loading clans..." );

    sprintf( clanlist, "%s%s", CLAN_DIR, CLAN_LIST );
    fclose( fpReserve );
    if ( ( fpList = fopen( clanlist, "r" ) ) == NULL )
    {
	perror( clanlist );
	exit( 1 );
    }

    for ( ; ; )
    {
	filename = feof( fpList ) ? "$" : fread_word( fpList );
	log_string( filename );
	if ( filename[0] == '$' )
	  break;

	if ( !load_clan_file( filename ) )
	{
	  sprintf( buf, "Cannot load clan file: %s", filename );
	  bug( buf, 0 );
	}
    }
    fclose( fpList );
    log_string(" Done clans " );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

/*
 * Load in all the council files.
 */
void load_councils( )
{
    FILE *fpList;
    char *filename;
    char councillist[256];
    char buf[MAX_STRING_LENGTH];


    first_council	= NULL;
    last_council	= NULL;

    log_string( "Loading councils..." );

    sprintf( councillist, "%s%s", COUNCIL_DIR, COUNCIL_LIST );
    fclose( fpReserve );
    if ( ( fpList = fopen( councillist, "r" ) ) == NULL )
    {
	perror( councillist );
	exit( 1 );
    }

    for ( ; ; )
    {
	filename = feof( fpList ) ? "$" : fread_word( fpList );
	log_string( filename );
	if ( filename[0] == '$' )
	  break;

	if ( !load_council_file( filename ) )
	{
	  sprintf( buf, "Cannot load council file: %s", filename );
	  bug( buf, 0 );
	}
    }
    fclose( fpList );
    log_string(" Done councils " );
    fpReserve = fopen( NULL_FILE, "r" );
    return;
}

void do_make( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    CLAN_DATA *clan;
    int level;

    if ( IS_NPC( ch ) || !ch->pcdata->clan )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    clan = ch->pcdata->clan;

    if ( str_cmp( ch->name, clan->leader )
    &&   str_cmp( ch->name, clan->deity )
    &&  (clan->clan_type != CLAN_GUILD
    ||   str_cmp( ch->name, clan->number1 )) )
    {
   send_to_char( "Como?\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
   send_to_char( "Crear que?\n\r", ch );
	return;
    }

    pObjIndex = get_obj_index( clan->clanobj1 );
    level = 40;

    if ( !pObjIndex || !is_name( arg, pObjIndex->name ) )
    {
      pObjIndex = get_obj_index( clan->clanobj2 );
      level = 45;
    }
    if ( !pObjIndex || !is_name( arg, pObjIndex->name ) )
    {
      pObjIndex = get_obj_index( clan->clanobj3 );
      level = 50;
    }
    if ( !pObjIndex || !is_name( arg, pObjIndex->name ) )
    {
      pObjIndex = get_obj_index( clan->clanobj4 );
      level = 35;
    }
    if ( !pObjIndex || !is_name( arg, pObjIndex->name ) )
    {
      pObjIndex = get_obj_index( clan->clanobj5 );
      level = 1;
    }

    if ( !pObjIndex || !is_name( arg, pObjIndex->name ) )
    {
   send_to_char( "No sabes como hacerlo.\n\r", ch );
	return;
    }

    obj = create_object( pObjIndex, level );
    xSET_BIT( obj->extra_flags, ITEM_CLANOBJECT );
    if ( CAN_WEAR(obj, ITEM_TAKE) )
      obj = obj_to_char( obj, ch );
    else
      obj = obj_to_room( obj, ch->in_room );
    act( AT_MAGIC, "$n crea $p!", ch, obj, NULL, TO_ROOM );
    act( AT_MAGIC, "Creas $p!", ch, obj, NULL, TO_CHAR );
    return;
}

void do_induct( CHAR_DATA *ch, char *argument )
{
    char elclan[256];
    char miembro[256];
    FILE *fichero;
    FILE *ficheroauxiliar;
    char arg[MAX_INPUT_LENGTH];
    int buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CLAN_DATA *clan;
    int i,bucle = 0;
   
    if ( IS_NPC( ch ) || !ch->pcdata->clan )
    {
   send_to_char( "Como?\n\r", ch );
	return;
    }

    clan = ch->pcdata->clan;

    if ( (ch->pcdata && ch->pcdata->bestowments
    &&    is_name("aceptar", ch->pcdata->bestowments))
    ||   !str_cmp( ch->name, clan->deity   )
    ||   !str_cmp( ch->name, clan->leader  )
    ||   !str_cmp( ch->name, clan->number1 )
    ||   !str_cmp( ch->name, clan->number2 ) )
	;
    else
    {
   send_to_char( "Como?\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
   send_to_char( "Admitir a quien?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
   send_to_char( "Ese jugador no esta aqui.\n\r", ch);
	return;
    }

    if ( IS_NPC(victim) )
    {
   send_to_char( "En un mob??\n\r", ch );
	return;
    }

    if ( IS_IMMORTAL(victim) )
    {
   send_to_char( "No puedes admitir a los dioses.\n\r", ch );
	return;
    }

    if ( clan->clan_type == CLAN_GUILD )
    {
	if ( victim->class != clan->class)
	{
       send_to_char( "No es de tu gremio.\n\r", ch);
            return;
	}
    }
    else
    {
	if ( victim->level < 10 )
	{
       send_to_char( "No deberias hacerle hacer eso a el precisamente.\n\r", ch );
	    return;
	}

    }

    if ( victim->pcdata->clan )
    {
      if ( victim->pcdata->clan->clan_type == CLAN_ORDER )
      {
	if ( victim->pcdata->clan == clan )
     send_to_char( "Este jugador ya pertenece a tu misma orden!\n\r", ch );
	else
     send_to_char( "El ya esta en una orden!\n\r", ch );
	return;
      }
      else
      if ( victim->pcdata->clan->clan_type == CLAN_GUILD )
      {
	if ( victim->pcdata->clan == clan )
     send_to_char( "Ya pertenece a tu gremio!\n\r", ch );
	else
     send_to_char( "Pertenece a otro gremio!\n\r", ch );
	return;
      }
      else
      {
	if ( victim->pcdata->clan == clan )
     send_to_char( "Lleva la misma sangre que tu en sus venas!\n\r", ch );
	else
     send_to_char( "El pertenece a otro clan!\n\r", ch );
	return;
      }
    }
    if ( clan->mem_limit && clan->members >= 200 )
    {
      send_to_char("Hay demasiada gente en tu familia.\n\r",ch);
	return;
    }
    clan->members++;
    if ( clan->clan_type != CLAN_ORDER && clan->clan_type != CLAN_GUILD )
      SET_BIT(victim->speaks, LANG_CLAN);

    if ( clan->clan_type != CLAN_NOKILL && clan->clan_type != CLAN_ORDER
    &&   clan->clan_type != CLAN_GUILD )
      SET_BIT( victim->pcdata->flags, PCFLAG_DEADLY );

    if ( clan->clan_type != CLAN_GUILD && clan->clan_type != CLAN_ORDER
    &&   clan->clan_type != CLAN_NOKILL )
    {
	int sn;


	for ( sn = 0; sn < top_sn; sn++ )
	{
	    if (skill_table[sn]->guild == clan->class &&
		skill_table[sn]->name != NULL )
	    {
		victim->pcdata->learned[sn] = GET_ADEPT(victim, sn);
      ch_printf( victim, "%s te adoctrina en el uso de %s.\n\r", ch->name, skill_table[sn]->name);
	    }
	}
    }


    victim->pcdata->clan = clan;
    STRFREE(victim->pcdata->clan_name);
    victim->pcdata->clan_name = QUICKLINK( clan->name );
    act( AT_ORANGE, "&g&gAceptas a &o$N &gcomo miembro de sangre de la familia $t", ch, clan->name, victim, TO_CHAR );
    act( AT_ORANGE, "$n &gnombra a &o$N &gcomo miembro de la familia $t", ch, clan->name, victim, TO_NOTVICT );
    act( AT_ORANGE, "$n &gte acepta como miembro de la familia $t", ch, clan->name, victim, TO_VICT );
    sprintf(buf, "%s &gha sido aceptado en la familia %s!", victim->name, clan->name);
    echo_to_all(AT_ORANGE, buf, ECHOTAR_ALL);
    
    /* Anyadido por Kayser 2004*/
    sprintf( elclan, "%s%s.aux",CLAN_DIR, clan->name);
    ficheroauxiliar = fopen (elclan, "w");
    fprintf(ficheroauxiliar,"%s\n", victim->name);
    fclose( ficheroauxiliar );
    ficheroauxiliar = fopen (elclan, "a");
    sprintf( elclan, "%s%s.txt",CLAN_DIR, clan->name);
    fichero = fopen (elclan, "r");
    for (i=0;i<50;i++)
    {
    	fscanf(fichero,"%s",miembro);
        fprintf(ficheroauxiliar,"%s\n",miembro);
	if (!strcmp(miembro,"~"))
        	break;
    }
    fclose(fichero);
    fclose( ficheroauxiliar );
    /* Fin Kayser 2004 */    
    /* Ahora a meter el contenido de *.aux a *.txt */
    sprintf( elclan, "%s%s.aux",CLAN_DIR, clan->name);
    ficheroauxiliar = fopen (elclan, "r");
    sprintf( elclan, "%s%s.txt",CLAN_DIR, clan->name);
    fichero = fopen (elclan, "w");
    for (i=0;i<50;i++)
        {
                fscanf(ficheroauxiliar,"%s",miembro);
                fprintf(fichero,"%s\n",miembro);
	        if (!strcmp(miembro,"~"))
                        break;
	}

    /* Fin de la copia de ficheros */
    fclose(fichero);
    fclose( ficheroauxiliar );

    save_char_obj( victim );
    save_clan( clan );
    return;
}

void do_council_induct( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    COUNCIL_DATA *council;

    if ( IS_NPC( ch ) || !ch->pcdata->council )
    {
   send_to_char_color( "&BY los&R tomates&B a komo van por ahi?\n\r", ch );
	return;
    }

    council = ch->pcdata->council;

  if ((council->head == NULL || str_cmp (ch->name, council->head))
      && ( council->head2 == NULL || str_cmp ( ch->name, council->head2 ))
      && str_cmp (council->name, "consejo mortal"))
    {
   send_to_char( "Ya te lo decia tu maestro... Nunca llegaras a nada.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
   send_to_char( "Aceptar a quien en que consejo?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
   send_to_char( "Ese jugador no esta en esta habitacion.\n\r", ch);
	return;
    }

    if ( IS_NPC(victim) )
    {
   send_to_char_color( "&RA los &YMob's&R?.\n\r", ch );
	return;
    }

/*    if ( victim->level < 51 )
    {
	send_to_char( "This player is not worthy of joining any council yet.\n\r", ch );
	return;
    }
*/
    if ( victim->pcdata->council )
    {
   send_to_char( "El ya pertenece a algun consejo!\n\r", ch );
	return;
    }

    council->members++;
    victim->pcdata->council = council;
    STRFREE(victim->pcdata->council_name);
    victim->pcdata->council_name = QUICKLINK( council->name );
    act( AT_MAGIC, "Has aceptado a $N dentro del $t", ch, council->name, victim, TO_CHAR );
    act( AT_MAGIC, "$n ha aceptado a $N dentro del $t", ch, council->name, victim, TO_ROOM );
    act( AT_MAGIC, "$n te ha acpetado como miembro del $t", ch, council->name, victim, TO_VICT );
    save_char_obj( victim );
    save_council( council );
    return;
}

void do_outcast( CHAR_DATA *ch, char *argument )
{
    char elclan[256];
    char miembro[256];
    FILE *fichero;
    FILE *ficheroauxiliar;
    int i;
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CLAN_DATA *clan;
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC( ch ) || !ch->pcdata->clan )
    {
   send_to_char_color( "&BSacate &Wlo&B que tienes en a boca para hablar.\n\r", ch );
	return;
    }

    clan = ch->pcdata->clan;

    if ( (ch->pcdata && ch->pcdata->bestowments
    &&    is_name("expulsar", ch->pcdata->bestowments))
    ||   !str_cmp( ch->name, clan->deity   )
    ||   !str_cmp( ch->name, clan->leader  )
    ||   !str_cmp( ch->name, clan->number1 )
    ||   !str_cmp( ch->name, clan->number2 ) )
	;
    else
    {
   send_to_char( "Como?\n\r", ch );
	return;
    }


    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
   send_to_char_color( "&WExpulsar a&R quien&W?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
   send_to_char( "No esta en la habitacion.\n\r", ch);
	return;
    }

    if ( IS_NPC(victim) )
    {
   send_to_char( "A un mob??\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	if ( ch->pcdata->clan->clan_type == CLAN_ORDER )
	{
       send_to_char( "Expulsarte a ti mismo? Tu estas seguro?\n\r", ch );
	    return;
	}
	else
	if ( ch->pcdata->clan->clan_type == CLAN_GUILD )
	{
       send_to_char( "Expulsarte a ti mismo? Tu estas seguro?\n\r", ch );
	    return;
	}
	else
	{
       send_to_char( "Expulsarte a ti mismo? Tu estas seguro?\n\r", ch );
	    return;
	}
    }

    if ( victim->pcdata->clan != ch->pcdata->clan )
    {
	if ( ch->pcdata->clan->clan_type == CLAN_ORDER )
	{
       send_to_char( "No pertenece a tu orden!\n\r", ch );
	    return;
	}
	else
	if ( ch->pcdata->clan->clan_type == CLAN_GUILD )
	{
       send_to_char( "No pertenece a tu consejo!\n\r", ch );
	    return;
	}
	else
	{
       send_to_char( "No pertenece a tu clan!\n\r", ch );
	    return;
	}
    }

    if ( clan->clan_type != CLAN_GUILD && clan->clan_type != CLAN_ORDER
    &&   clan->clan_type != CLAN_NOKILL )
    {
	int sn;

	for ( sn = 0; sn < top_sn; sn++ )
	    if ( skill_table[sn]->guild == victim->pcdata->clan->class
	    &&   skill_table[sn]->name != NULL )
	    {
		victim->pcdata->learned[sn] = 0;
      ch_printf( victim, "Olvidas como se hacia %s.\n\r", skill_table[sn]->name);
	    }
    }

    if ( victim->speaking & LANG_CLAN )
        victim->speaking = LANG_COMMON;
    REMOVE_BIT( victim->speaks, LANG_CLAN );
    --clan->members;
    if ( !str_cmp( victim->name, ch->pcdata->clan->number1 ) )
    {
	STRFREE( ch->pcdata->clan->number1 );
	ch->pcdata->clan->number1 = STRALLOC( "" );
    }
    if ( !str_cmp( victim->name, ch->pcdata->clan->number2 ) )
    {
	STRFREE( ch->pcdata->clan->number2 );
	ch->pcdata->clan->number2 = STRALLOC( "" );
    }
    victim->pcdata->clan = NULL;
    STRFREE(victim->pcdata->clan_name);
    victim->pcdata->clan_name = STRALLOC( "" );
    act( AT_MAGIC, "Expulsas a $N del clan $t", ch, clan->name, victim, TO_CHAR );
    act( AT_MAGIC, "$n expulsa a $N del clan $t", ch, clan->name, victim, TO_ROOM );
    act( AT_MAGIC, "$n te han expulsado del clan $t", ch, clan->name, victim, TO_VICT );
    if ( clan->clan_type != CLAN_GUILD
    &&   clan->clan_type != CLAN_ORDER )
    {
   sprintf(buf, "%s ha sido expulsado del clan %s!", victim->name, clan->name);
	echo_to_all(AT_MAGIC, buf, ECHOTAR_ALL);
    }
    sprintf( elclan, "%s%s.aux",CLAN_DIR, clan->name);
    ficheroauxiliar = fopen (elclan, "w");
    sprintf( elclan, "%s%s.txt",CLAN_DIR, clan->name);
    fichero = fopen (elclan, "r");
    for (i=0;i<50;i++)
    {
        fscanf(fichero,"%s",miembro);
	if (strcmp(miembro,victim->name))	
	        fprintf(ficheroauxiliar,"%s\n",miembro);
        if (!strcmp(miembro,"~"))
                break;
    }
    fclose( fichero );
    fclose( ficheroauxiliar );
    /* Ahora a meter el contenido de *.aux a *.txt */
    sprintf( elclan, "%s%s.aux",CLAN_DIR, clan->name);
    ficheroauxiliar = fopen (elclan, "r");
    sprintf( elclan, "%s%s.txt",CLAN_DIR, clan->name);
    fichero = fopen (elclan, "w");
    for (i=0;i<50;i++)
        {
                fscanf(ficheroauxiliar,"%s",miembro);
                fprintf(fichero,"%s\n",miembro);
                if (!strcmp(miembro,"~"))
                        break;
        }

    /* Fin de la copia de ficheros */
    fclose( fichero );
    fclose( ficheroauxiliar );
    save_char_obj( victim );	/* clan gets saved when pfile is saved */
    save_clan( clan );
    return;
}

void do_council_outcast( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    COUNCIL_DATA *council;

    if ( IS_NPC( ch ) || !ch->pcdata->council )
    {
   send_to_char( "Como?\n\r", ch );
	return;
    }

    council = ch->pcdata->council;

  if ((council->head == NULL || str_cmp (ch->name, council->head))
      && ( council->head2 == NULL || str_cmp ( ch->name, council->head2 ))
      && str_cmp (council->name, "consejo mortal"))
    {
   send_to_char( "Anda!! Si hablas Japones y to!!\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
   send_to_char( "Expulsar a quien del consejo?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
   send_to_char( "No esta en esta habitacon.\n\r", ch);
	return;
    }

    if ( IS_NPC(victim) )
    {
   send_to_char( "No en jugadores.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
   send_to_char( "Expulsarte a ti mismo del consejo? Nooo...\n\r", ch );
	return;
    }

    if ( victim->pcdata->council != ch->pcdata->council )
    {
   send_to_char( "No pertece a tu consejo...\n\r", ch );
	return;
    }

    --council->members;
    victim->pcdata->council = NULL;
    STRFREE(victim->pcdata->council_name);
    victim->pcdata->council_name = STRALLOC( "" );
    act( AT_MAGIC, "Expulsas a $N del $t", ch, council->name, victim, TO_CHAR );
    act( AT_MAGIC, "$n expulsa a $N del $t", ch, council->name, victim, TO_ROOM );
    act( AT_MAGIC, "$n te ha expulsado del $t", ch, council->name, victim, TO_VICT );
    save_char_obj( victim );
    save_council( council );
    return;
}

void do_setclan( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CLAN_DATA *clan;

    set_char_color( AT_PLAIN, ch );
    if ( IS_NPC( ch ) )
    {
   send_to_char( "Los churros con guiski tan guenos?\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Uso: setfamilia <familia> <campo> <protector|lider|heredero|consejero> <jugador>\n\r", ch );
	send_to_char( "\n\rEstos son los campos:\n\r", ch );
	send_to_char( " protector lider heredero consejero\n\r", ch );
	send_to_char( " miembros tablero recall almacen guardia1 guardia2\n\r", ch );
	send_to_char( " memlimit leadrank onerank tworank\n\r", ch );
	send_to_char( " obj1 obj2 obj3 obj4 obj5\n\r", ch );
	if ( get_trust( ch ) >= LEVEL_GOD )
	{
	  send_to_char( " nombre filename apodo desc\n\r", ch );
	  send_to_char( " favor golpes tipo clase\n\r", ch );
	}
	if ( get_trust( ch ) >= LEVEL_IMPLEMENTOR )
	  send_to_char(" mkills mdeaths\n\r limpiar = Poner a 0 el clan \n\r&RNO USAR SI NO SE ESTA SEGURO NO SE PUEDE DESACER", ch );
	return;
    }

    clan = get_clan( arg1 );
    if ( !clan )
    {
	send_to_char( "Esa familia no existe ( borracho de mierda ).\n\r", ch );
	return;
    }
    if ( !str_prefix( arg2, "protector" ) )
    {
	STRFREE( clan->deity );
	clan->deity = STRALLOC( argument );
	send_to_char( "Marchando una de papas.\n\r", ch );
	save_clan( clan );
	return;
    }

    if ( !str_prefix( arg2, "puntos" ) )
      {
        clan->puntos = atoi( argument );
        send_to_char("Hecho, &Rpero kuidadin kon los puntos\n\r", ch);
        save_clan( clan );
        return;
      }

   if ( !str_prefix( arg2, "numero" ) )
   {
             clan->numero = atoi( argument );
             send_to_char( "Hecho berberecho.\n\r", ch );
             save_clan( clan );
             return;
    }

    if ( !str_prefix( arg2, "limpiar" ) )
      {

	clan->members = 0;
        clan->mkills = 0;
        clan->mdeaths = 0;
        clan->puntos = 0;
        clan->conquistas = 0;
        send_to_char( "Hecho...\n\r", ch);
        bug("Familia %s puesta a cero\n\r", clan->name);
        save_clan( clan );
        do_showclan( ch, clan);
        return;
      }

    if ( !str_prefix( arg2, "mkills" ) )
      {
	clan->mkills = atoi( argument );
        send_to_char("Ok!\n\r",ch);
        save_clan( clan );
        return;
      }

    if (!str_prefix( arg2, "mdeaths" ) )
      {
	clan->mdeaths = atoi( argument );
        send_to_char("Ok!\n\r", ch);
        save_clan( clan );
        return;
      }

     if ( !str_prefix( arg2, "lider" ) )
    {
	STRFREE( clan->leader );
	clan->leader = STRALLOC( argument );
	send_to_char( "Puedo ofrecerle una copa de vino?.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_prefix( arg2, "heredero" ) )
    {
	STRFREE( clan->number1 );
	clan->number1 = STRALLOC( argument );
	send_to_char( "Muy hecho o poko hecho?.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_prefix( arg2, "consejero" ) )
    {
	STRFREE( clan->number2 );
	clan->number2 = STRALLOC( argument );
	send_to_char( "Acompanyameeeee dejate llevaaaaaaaaar.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_prefix( arg2, "leadrank" ) )
    {
	STRFREE( clan->leadrank );
	clan->leadrank = STRALLOC( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_prefix( arg2, "onerank" ) )
    {
	STRFREE( clan->onerank );
	clan->onerank = STRALLOC( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_prefix( arg2, "tworank" ) )
    {
	STRFREE( clan->tworank );
	clan->tworank = STRALLOC( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_prefix( arg2, "badge" ) )
    {
	STRFREE( clan->badge );
	clan->badge = STRALLOC( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_prefix( arg2, "tablero" ) )
    {
	clan->board = atoi( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_prefix( arg2, "memlimit") )
    {
    	clan->mem_limit = atoi( argument );
	send_to_char( "Done.\n\r", ch  );
	save_clan( clan );
	return;
    }
    if ( !str_prefix( arg2, "miembros" ) )
    {
	clan->members = atoi( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_prefix( arg2, "recall" ) )
    {
	clan->recall = atoi( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_prefix( arg2, "almacen" ) )
    {
	clan->storeroom = atoi( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_prefix( arg2, "obj1" ) )
    {
	clan->clanobj1 = atoi( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_cmp( arg2, "obj2" ) )
    {
	clan->clanobj2 = atoi( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_cmp( arg2, "obj3" ) )
    {
	clan->clanobj3 = atoi( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_cmp( arg2, "obj4" ) )
    {
        clan->clanobj4 = atoi( argument );
        send_to_char( "Done.\n\r", ch );
        save_clan( clan );
        return;
    }
    if ( !str_cmp( arg2, "obj5" ) )
    {
        clan->clanobj5 = atoi( argument );
        send_to_char( "Done.\n\r", ch );
        save_clan( clan );
        return;
    }
    if ( !str_prefix( arg2, "guardia1" ) )
    {
	clan->guard1 = atoi( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_cmp( arg2, "guardia2" ) )
    {
	clan->guard2 = atoi( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( get_trust( ch ) < LEVEL_GOD )
    {
	do_setclan( ch, "" );
	return;
    }
    if ( !str_cmp( arg2, "align" ) )
    {
	clan->alignment = atoi( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_prefix( arg2, "tipo" ) )
    {
	if ( !str_prefix( argument, "order" ) )
	  clan->clan_type = CLAN_ORDER;
	else
	if ( !str_prefix( argument, "guild" ) )
	  clan->clan_type = CLAN_GUILD;
	else
	  clan->clan_type = atoi( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_prefix( arg2, "clase" ) )
    {
	clan->class = atoi( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_prefix( arg2, "nombre" ) )
    {
	STRFREE( clan->name );
	clan->name = STRALLOC( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_prefix( arg2, "filename" ) )
    {
	DISPOSE( clan->filename );
                 send_to_char( "Hago el DISPOSE\n\r", ch );
	clan->filename = str_dup( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	write_clan_list( );
	return;
    }
    
    /*
     * SiGo, esto lo anyado para poner la morgue del clan, que ya estaba
     * hecho pero alguien me lo ha chafado.
     */
    
    if( !str_prefix( arg2, "morgue" ) )
    {
    	clan->morgue = atoi( argument );
	send_to_char( "Hecho.\n\r", ch );
	save_clan( clan );
	return;
    }

    if ( !str_prefix( arg2, "apodo" ) )
    {
	STRFREE( clan->motto );
	clan->motto = STRALLOC( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( !str_prefix( arg2, "desc" ) )
    {
	STRFREE( clan->description );
	clan->description = STRALLOC( argument );
	send_to_char( "Done.\n\r", ch );
	save_clan( clan );
	return;
    }
    if ( get_trust( ch ) < LEVEL_IMPLEMENTOR )
    {
        do_setclan( ch, "" );
        return;
    }
    if ( !str_prefix( "pkill", arg2) )
    {
	int temp_value;
	if ( !str_cmp( arg2, "pkill1" ) )
		temp_value = 0;
	else if ( !str_cmp( arg2, "pkill2" ) )
		temp_value = 1;
	else if ( !str_cmp( arg2, "pkill3" ) )
		temp_value = 2;
	else if ( !str_cmp( arg2, "pkill4" ) )
		temp_value = 3;
	else if ( !str_cmp( arg2, "pkill5" ) )
		temp_value = 4;
	else if ( !str_cmp( arg2, "pkill6" ) )
		temp_value = 5;
	else if ( !str_cmp( arg2, "pkill7" ) )
		temp_value = 6;
	else
	{
		do_setclan ( ch, "" );
		return;
	}
	clan->pkills[temp_value] = atoi( argument );
	send_to_char ("Ok.\n\r", ch );
	return;
    }
    if ( !str_prefix( "pdeath", arg2) )
    {
	int temp_value;
	if ( !str_cmp( arg2, "pdeath1" ) )
		temp_value = 0;
	else if ( !str_cmp( arg2, "pdeath2" ) )
		temp_value = 1;
	else if ( !str_cmp( arg2, "pdeath3" ) )
		temp_value = 2;
	else if ( !str_cmp( arg2, "pdeath4" ) )
		temp_value = 3;
	else if ( !str_cmp( arg2, "pdeath5" ) )
		temp_value = 4;
	else if ( !str_cmp( arg2, "pdeath6" ) )
		temp_value = 5;
	else if ( !str_cmp( arg2, "pdeath7" ) )
		temp_value = 6;
	else
	{
		do_setclan ( ch, "" );
		return;
	}
	clan->pdeaths[temp_value] = atoi( argument );
	send_to_char ("Ok.\n\r", ch );
	return;
    }
    do_setclan( ch, "" );
    return;
}

void do_setcouncil( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    COUNCIL_DATA *council;

    set_char_color( AT_PLAIN, ch );

    if ( IS_NPC( ch ) )
    {
   send_to_char( "En Alaska hace frio?\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( arg1[0] == '\0' )
    {
   send_to_char( "Uso: setconsejo <council> <field> <deity|leader|number1|number2> <player>\n\r", ch );
	send_to_char( "\n\rField being one of:\n\r", ch );
	send_to_char( " head head2 members board meeting\n\r", ch );
	if ( get_trust( ch ) >= LEVEL_GOD )
	  send_to_char( " name filename desc\n\r", ch );
        if ( get_trust( ch ) >= LEVEL_SUB_IMPLEM )
	  send_to_char( " powers\n\r", ch);
	return;
    }

    council = get_council( arg1 );
    if ( !council )
    {
	send_to_char( "No such council.\n\r", ch );
	return;
    }
    if ( !str_cmp( arg2, "head" ) )
    {
	STRFREE( council->head );
	council->head = STRALLOC( argument );
	send_to_char( "Done.\n\r", ch );
	save_council( council );
	return;
    }

  if (!str_cmp (arg2, "head2"))
    {
      if ( council->head2 != NULL )
        STRFREE (council->head2);
      if ( !str_cmp ( argument, "none" ) || !str_cmp ( argument, "clear" ) )
        council->head2 = NULL;
      else
        council->head2 = STRALLOC (argument);
      send_to_char ("Done.\n\r", ch);
      save_council (council);
      return;
    }
    if ( !str_cmp( arg2, "board" ) )
    {
	council->board = atoi( argument );
	send_to_char( "Done.\n\r", ch );
	save_council( council );
	return;
    }
    if ( !str_cmp( arg2, "members" ) )
    {
	council->members = atoi( argument );
	send_to_char( "Done.\n\r", ch );
	save_council( council );
	return;
    }
    if ( !str_cmp( arg2, "meeting" ) )
    {
	council->meeting = atoi( argument );
	send_to_char( "Done.\n\r", ch );
	save_council( council );
	return;
    }
    if ( get_trust( ch ) < LEVEL_GOD )
    {
	do_setcouncil( ch, "" );
	return;
    }
    if ( !str_cmp( arg2, "name" ) )
    {
	STRFREE( council->name );
	council->name = STRALLOC( argument );
	send_to_char( "Done.\n\r", ch );
	save_council( council );
	return;
    }
    if ( !str_cmp( arg2, "filename" ) )
    {
	DISPOSE( council->filename );
	council->filename = str_dup( argument );
	send_to_char( "Done.\n\r", ch );
	save_council( council );
	write_council_list( );
	return;
    }
    if ( !str_cmp( arg2, "desc" ) )
    {
	STRFREE( council->description );
	council->description = STRALLOC( argument );
	send_to_char( "Done.\n\r", ch );
	save_council( council );
	return;
    }
    if ( get_trust( ch ) < LEVEL_SUB_IMPLEM )
    {
	do_setcouncil( ch, "" );
	return;
    }
    if ( !str_cmp( arg2, "powers" ) )
    {
	STRFREE( council->powers );
	council->powers = STRALLOC( argument );
	send_to_char( "Done.\n\r", ch );
	save_council( council );
	return;
    }

    do_setcouncil( ch, "" );
    return;
}

/*
 * Added multiple levels on pkills and pdeaths. -- Shaddai
 */

void do_showclan( CHAR_DATA *ch, char *argument )
{
    CLAN_DATA *clan;

    set_char_color( AT_PLAIN, ch );

    if ( IS_NPC( ch ) )
    {
   send_to_char( "Como?\n\r", ch );
	return;
    }
    if ( argument[0] == '\0' )
    {
	send_to_char( "Usage: showclan <clan>\n\r", ch );
	return;
    }

    clan = get_clan( argument );
    if ( !clan )
    {
   send_to_char( "Eso no es un clan una orden o un gremio.\n\r", ch );
	return;
    }

    ch_printf_color( ch, "\n\r&w%s    : &W%s\t\t&wBadge: %s\n\r&wFilename : &W%s\n\r&wMotto    : &W%s\n\r",
			clan->clan_type == CLAN_ORDER ? "Order" :
		       (clan->clan_type == CLAN_GUILD ? "Guild" : "Clan "),
    			clan->name,
			clan->badge ? clan->badge : "(not set)",
    			clan->filename,
    			clan->motto );
    ch_printf_color( ch, "&wDesc     : &W%s\n\r&wDeity    : &W%s\n\r",
    			clan->description,
    			clan->deity );
    ch_printf_color( ch, "&wLeader   : &W%-19.19s\t&wRank: &W%s\n\r",
			clan->leader,
			clan->leadrank );
    ch_printf_color( ch, "&wNumber1  : &W%-19.19s\t&wRank: &W%s\n\r",
			clan->number1,
			clan->onerank );
    ch_printf_color( ch, "&wNumber2  : &W%-19.19s\t&wRank: &W%s\n\r",
			clan->number2,
			clan->tworank );
    ch_printf_color( ch, "&wPKills   : &w1-9:&W%-3d &w10-14:&W%-3d &w15-19:&W%-3d &w20-29:&W%-3d &w30-39:&W%-3d &w40-49:&W%-3d &w50:&W%-3d\n\r",
    			clan->pkills[0], clan->pkills[1], clan->pkills[2],
    			clan->pkills[3], clan->pkills[4], clan->pkills[5],
			clan->pkills[6]);
    ch_printf_color( ch, "&wPDeaths  : &w1-9:&W%-3d &w10-14:&W%-3d &w15-19:&W%-3d &w20-29:&W%-3d &w30-39:&W%-3d &w40-49:&W%-3d &w50:&W%-3d\n\r",
    			clan->pdeaths[0], clan->pdeaths[1], clan->pdeaths[2],
    			clan->pdeaths[3], clan->pdeaths[4], clan->pdeaths[5],
			clan->pdeaths[6] );
    ch_printf_color( ch, "&wIllegalPK: &W%-6d\n\r",
			clan->illegal_pk );
    ch_printf_color( ch, "&wMKills   : &W%-6d   &wMDeaths: &W%-6d\n\r",
    			clan->mkills,
    			clan->mdeaths );
    ch_printf_color( ch, "&wScore    : &W%-6d   &wFavor  : &W%-6d   &wStrikes: &W%d\n\r",
    			clan->score,
    			clan->favour,
    			clan->strikes );
    ch_printf_color( ch, "&wMembers  : &W%-6d  &wMemLimit: &W%-6d   &wAlign  : &W%-6d",
    			clan->members,
    			clan->mem_limit,
    			clan->alignment );
    if ( clan->clan_type == CLAN_GUILD )
	ch_printf_color( ch, "   &wClass  : &W%d &w(&W%s&w)",
			clan->class,
    			clan->class<MAX_PC_CLASS?class_table[clan->class]->who_name:
			"unknown" );
    send_to_char( "\n\r", ch );
    ch_printf_color( ch, "&wBoard    : &W%-5d    &wRecall : &W%-5d    &wStorage: &W%-5d\n\r",
			clan->board,
			clan->recall,
			clan->storeroom );
    ch_printf_color( ch, "&wGuard1   : &W%-5d    &wGuard2 : &W%-5d\n\r",
 			clan->guard1,
			clan->guard2 );
    ch_printf_color( ch, "&wObj1( &W%d &w)  Obj2( &W%d &w)  Obj3( &W%d &w)  Obj4( &W%d &w)  Obj5( &W%d &w)\n\r",
    			clan->clanobj1,
    			clan->clanobj2,
    			clan->clanobj3,
			clan->clanobj4,
			clan->clanobj5 );
    ch_printf( ch, "&wMorgue de la Familia : &g%d	&wOnzas :&Y%d\n\r", clan->morgue, clan->oro );
    return;
}

void do_showcouncil( CHAR_DATA *ch, char *argument )
{
    COUNCIL_DATA *council;

    set_char_color( AT_PLAIN, ch );

    if ( IS_NPC( ch ) )
    {
   send_to_char( "Como?\n\r", ch );
	return;
    }
    if ( argument[0] == '\0' )
    {
   send_to_char( "Uso: verconsejo <consejo>\n\r", ch );
	return;
    }

    council = get_council( argument );
    if ( !council )
    {
   send_to_char( "No es un consejo.\n\r", ch );
	return;
    }

    ch_printf_color( ch, "\n\r&wCouncil :  &W%s\n\r&wFilename:  &W%s\n\r",
    			council->name,
    			council->filename );
  ch_printf_color (ch, "&wHead:      &W%s\n\r", council->head );
  ch_printf_color (ch, "&wHead2:     &W%s\n\r", council->head2 );
  ch_printf_color (ch, "&wMembers:   &W%-d\n\r", council->members );
    ch_printf_color( ch, "&wBoard:     &W%-5d\n\r&wMeeting:   &W%-5d\n\r&wPowers:    &W%s\n\r",
    			council->board,
    			council->meeting,
			council->powers );
    ch_printf_color( ch, "&wDescription:\n\r&W%s\n\r", council->description );
    return;
}

void do_makeclan( CHAR_DATA *ch, char *argument )
{
    char filename[256];
    CLAN_DATA *clan;
    bool found;
    int bucle;

    set_char_color( AT_IMMORT, ch );

    if ( !argument || argument[0] == '\0' )
    {
	send_to_char( "Usage: makeclan <clan name>\n\r", ch );
	return;
    }

    found = FALSE;
    sprintf( filename, "%s%s", CLAN_DIR, strlower(argument) );

    CREATE( clan, CLAN_DATA, 1 );
    LINK( clan, first_clan, last_clan, next, prev );

    clan->name		= STRALLOC( argument );
    clan->filename	= STRALLOC( "" ); /*Bug fix by baria@mud.tander.com*/
    clan->motto		= STRALLOC( "" );
    clan->description	= STRALLOC( "" );
    clan->deity		= STRALLOC( "" );
    clan->leader	= STRALLOC( "" );
    clan->number1	= STRALLOC( "" );
    clan->number2	= STRALLOC( "" );
    clan->leadrank	= STRALLOC( "" );
    clan->onerank	= STRALLOC( "" );
    clan->tworank	= STRALLOC( "" );
    clan->badge		= STRALLOC( "" );
    clan->numero = 666;
}

void do_makecouncil( CHAR_DATA *ch, char *argument )
{
    char filename[256];
    COUNCIL_DATA *council;
    bool found;

    set_char_color( AT_IMMORT, ch );

    if ( !argument || argument[0] == '\0' )
    {
   send_to_char( "Uso: crearconsejo <nombre consejo>\n\r", ch );
	return;
    }

    found = FALSE;
    sprintf( filename, "%s%s", COUNCIL_DIR, strlower(argument) );

    CREATE( council, COUNCIL_DATA, 1 );
    LINK( council, first_council, last_council, next, prev );
    council->name		= STRALLOC( argument );
    council->head		= STRALLOC( "" );
    council->head2 		= NULL;
    council->powers		= STRALLOC( "" );
}

/*
 * Added multiple level pkill and pdeath support. --Shaddai
 */

void do_clans( CHAR_DATA *ch, char *argument )
{
    CLAN_DATA *clan;
    CLAN_DATA *pclan;
    int count = 0;
    int pwr;
    int acon = 0;
    int pcon = 0;
    char poder[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    AREA_DATA       *pArea;
    CON_DATA *conquista;
    int p, a;
    char pos[MAX_STRING_LENGTH];
    int npos;


    if ( argument[0] == '\0' )
    {
      ch_printf( ch, "&gFamilia    &w       |&g Lider  &w     |&g Miembros&w | &gConq  &w|&g Puntos&w |&g M.C. &w |&g A.C.&w |&g Pos&w\n\r------------------|-------------|----------|-------|--------|-------|------|-----\n\r" );
        for ( clan = first_clan; clan; clan = clan->next )
        {
            if ( clan->clan_type == CLAN_ORDER || clan->clan_type == CLAN_GUILD )
              continue;
            ch_printf( ch, "&c%-17.17s&w | &g%-11.11s&w |      &g%-3.3d &w|&g %-5.5d &w| &g%-5.5d &w |", clan->name, clan->leader, clan->members, clan->conquistas, clan->puntos );
            count++;

	    // Amos a ver que pasa
	    npos = 4;
	    for( pclan = first_clan; pclan; pclan = pclan->next )
	      {
		if( pclan->name != clan->name )		  
		    if( pclan->puntos < clan->puntos )
		      npos--;
	      }
	    
	    if( npos == 1 ) sprintf( pos, "&Y-=1&Y=-" );
	    if( npos == 2 ) sprintf( pos, "&W-=2&W=-" );
	    if( npos == 3 ) sprintf( pos, "&R**3&R**" );
	    if( npos == 4 ) sprintf( pos, "&B**4&B**" );

            if ( ch->pcdata->clan != NULL )
	    {
	    pcon = 0;
            for ( d = first_descriptor; d; d= d->next)
	      {
		if ( d->character->pcdata->clan != NULL && d->character->pcdata->clan->name == clan->name)
		  pcon++;
		else
		  continue;
	      }
             for ( pArea = first_area; pArea; pArea = pArea->next )
	      {
		conquista = get_conquista( pArea->name );
		if( IS_CLANNED(ch))
		if(( conquista->propietario == ch->pcdata->clan->name ) && ch->pcdata->clan->name == clan->name)
		  acon++;
	      }
	     if( IS_CLANNED(ch))
            if (( ch->pcdata->clan->name == clan->name ))
	      ch_printf( ch, "&g %-5.5d &w| &g%-5.5d&w|%s\n\r", pcon, acon, pos);
	    else
	      ch_printf( ch, "-------|------|%s\n\r", pos );
         }
	    if( ch->pcdata->clan == NULL )
	      ch_printf( ch, "-------|------|%s\n\r", pos );
	}
        if ( !count )
          ch_printf( ch, "&WAun no se ha formado ninguna familia.\n\r" );
        else
          send_to_char( "&w__________________________________________________________________________________\n\r\n\r", ch);
          ch_printf( ch, "&wHay en total &r%d &rfamilias formadas.\n\r", count);
          ch_printf( ch, "&WA.C. &wAreas Conquistadas ahora. &WM.C. &wMiembros conectados ahora.\n\r" );
          send_to_char( "&gUso 'familias <familia>' para una informacion detallada de sus victorias.\n\r", ch );
          return;
    }

    clan = get_clan( argument );
    if ( !clan || clan->clan_type == CLAN_GUILD || clan->clan_type == CLAN_ORDER )
    {
        set_char_color( AT_BLOOD, ch );
        send_to_char( "No es una familia.\n\r", ch );
        return;
    }

    ch_printf( ch, "&g@..............oO0Oo &wFaMiLia %-15.15s &goO0Oo..............@\n\r",clan->name );
    ch_printf( ch, "&g*                                                               *\n\r" );
    ch_printf( ch, "&g* &bLider: &w%-12.12s            &bDeidad: &w%-12.12s           &g*\n\r", clan->leader, clan->deity );
    ch_printf( ch, "&g* &bHeredero: &w%-12.12s         &bConsejero: &w%-12.12s        &g*\n\r", clan->number1, clan->number2 );
    ch_printf( ch, "&g*                                                               *\n\r" );
    ch_printf( ch, "&g*-----------------------------==O==-----------------------------*\n\r" );
    ch_printf( ch, "&g* &bMiembros: &w%-3.3d                  &bPuntos: &w%-5.5d                  &g*\n\r", clan->members, clan->puntos );
    ch_printf( ch, "&g*-----------------------------==O==-----------------------------*\n\r" );
    if( IS_CLANNED(ch))
    if (( ch->pcdata->clan->name == clan->name ) || IS_IMMORTAL(ch))
    {
    ch_printf( ch, "&g*                                                               *\n\r" );
    ch_printf( ch, "&g* &bConquistas: &w%5d              &bTierras: &w%6d                &g*\n\r", clan->conquistas, clan->tierras );
    ch_printf( ch, "&g*                                                               *\n\r" );
    ch_printf( ch, "&g*-----------------------------==O==-----------------------------*\n\r" );
    }

    return;
}

void do_orders( CHAR_DATA *ch, char *argument )
{
    CLAN_DATA *order;
    int count = 0;

    if ( argument[0] == '\0' )
    {
        set_char_color( AT_DGREEN, ch );
        send_to_char( "\n\rOrden            Deidad          Lider           Mkills      Mdeaths\n\r____________________________________________________________________\n\r\n\r", ch );
	set_char_color( AT_GREEN, ch );
        for ( order = first_clan; order; order = order->next )
        if ( order->clan_type == CLAN_ORDER )
        {
            ch_printf( ch, "%-16s %-14s %-14s   %-7d       %5d\n\r",
	      order->name, order->deity, order->leader, order->mkills, order->mdeaths );
            count++;
	}
        set_char_color( AT_DGREEN, ch );
	if ( !count )
     send_to_char( "Aun no se han formado Ordenes.\n\r", ch );
	else
     send_to_char( "____________________________________________________________________\n\r\n\rUso 'ordenes <orden>' para mas detalles.\n\r", ch );
	return;
    }

    order = get_clan( argument );
    if ( !order || order->clan_type != CLAN_ORDER )
    {
        set_char_color( AT_DGREEN, ch );
        send_to_char( "No such Order.\n\r", ch );
        return;
    }

    set_char_color( AT_DGREEN, ch );
    ch_printf( ch, "\n\rOrder of %s\n\r'%s'\n\r\n\r", order->name, order->motto );
    set_char_color( AT_GREEN, ch );
    ch_printf( ch, "Deidad      :  %s\n\rLider     :  %s\n\rNumero Uno :  %s\n\rNumero Dos :  %s\n\r",
                        order->deity,
                        order->leader,
                        order->number1,
                        order->number2 );
    if ( !str_cmp( ch->name, order->deity   )
    ||   !str_cmp( ch->name, order->leader  )
    ||   !str_cmp( ch->name, order->number1 )
    ||   !str_cmp( ch->name, order->number2 ) )
        ch_printf( ch, "Miembros    :  %d\n\r",
			order->members );
    set_char_color( AT_DGREEN, ch );
    ch_printf( ch, "\n\rDescripcion:\n\r%s\n\r", order->description );
    return;
}

void do_councils( CHAR_DATA *ch, char *argument)
{
    COUNCIL_DATA *council;

    set_char_color( AT_CYAN, ch );
    if ( !first_council )
    {
   send_to_char( "Aun no hay consejos formados.\n\r", ch );
	return;
    }
    if ( argument[0] == '\0' )
    {
        send_to_char_color ("\n\r&cTitulo                    Cabeza\n\r", ch);
      for (council = first_council; council; council = council->next)
      {
        if ( council->head2 != NULL )
           ch_printf_color (ch, "&w%-24s %s and %s\n\r",  council->name,
                council->head, council->head2 );
        else
           ch_printf_color (ch, "&w%-24s %-14s\n\r", council->name, council->head);
      }
      send_to_char_color( "&cUso 'consejos <monbre del consejo>' para nformacion detallada.\n\r", ch );
      return;
    }
    council = get_council( argument );
    if ( !council )
    {
      send_to_char_color( "&cEse consejo no existe...\n\r", ch );
      return;
    }
    ch_printf_color( ch, "&c\n\r%s\n\r", council->name );
  if ( council->head2 == NULL )
        ch_printf_color (ch, "&cCabeza:     &w%s\n\r&cMiembros:  &w%d\n\r",
        council->head, council->members );
  else
        ch_printf_color (ch, "&cCo-Cabezas:     &w%s &cand &w%s\n\r&cMiembros:  &w%d\n\r",
           council->head, council->head2, council->members );
    ch_printf_color( ch, "&cDescripcion:\n\r&w%s\n\r",
        council->description );
    return;
}

void do_guilds( CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    CLAN_DATA *guild;
    int count = 0;

    if ( argument[0] == '\0' )
    {
        set_char_color( AT_HUNGRY, ch );
        send_to_char( "\n\rGremio                  Lider             Mkills      Mdeaths\n\r_____________________________________________________________\n\r\n\r", ch );
	set_char_color( AT_YELLOW, ch );
        for ( guild = first_clan; guild; guild = guild->next )
        if ( guild->clan_type == CLAN_GUILD )
	{
	    ++count;
	    ch_printf( ch, "%-20s   %-14s     %-6d       %6d\n\r", guild->name, guild->leader, guild->mkills, guild->mdeaths );
	}
        set_char_color( AT_HUNGRY, ch );
        if ( !count )
          send_to_char( "Aun no hay gremios creados.\n\r", ch );
        else
	  send_to_char( "_____________________________________________________________\n\r\n\rUso gremio 'clase' para informacion especifica. (ex:  guilds thieves)\n\r", ch );
	return;
    }

    sprintf( buf, "gremio de %s", argument );
    guild = get_clan( buf );
    if ( !guild || guild->clan_type != CLAN_GUILD )
    {
        set_char_color( AT_HUNGRY, ch );
        send_to_char( "No es un Gremio.\n\r", ch );
        return;
    }
    set_char_color( AT_HUNGRY, ch );
    ch_printf( ch, "\n\r%s\n\r", guild->name );
    set_char_color( AT_YELLOW, ch );
    ch_printf( ch, "Lider:    %s\n\rNumero 1:  %s\n\rNumero 2:  %s\n\rMotto:     %s\n\r",
                        guild->leader,
                        guild->number1,
                        guild->number2,
			guild->motto );
    if ( !str_cmp( ch->name, guild->deity   )
    ||   !str_cmp( ch->name, guild->leader  )
    ||   !str_cmp( ch->name, guild->number1 )
    ||   !str_cmp( ch->name, guild->number2 ) )
        ch_printf( ch, "Members:   %d\n\r",
			guild->members );
    set_char_color( AT_HUNGRY, ch );
    ch_printf( ch, "Guild Description:\n\r%s\n\r", guild->description );
    return;
}

void do_victories( CHAR_DATA *ch, char *argument )
{
    char filename[256];

    if ( IS_NPC( ch ) || !ch->pcdata->clan ) {
        send_to_char( "Como?\n\r", ch );
        return;
    }
    if ( ch->pcdata->clan->clan_type != CLAN_ORDER
    &&   ch->pcdata->clan->clan_type != CLAN_GUILD ) {
        sprintf( filename, "%s%s.record",
          CLAN_DIR, ch->pcdata->clan->name );
	set_pager_color( AT_PURPLE, ch );
	if ( !str_cmp( ch->name, ch->pcdata->clan->leader )
	&&   !str_cmp( argument, "clean" ) ) {
	  FILE *fp = fopen( filename, "w" );
	  if ( fp )
	    fclose( fp );
     send_to_pager( "\n\rVictorias han sido borradas.\n\r", ch );
	  return;
	} else {
     send_to_pager( "\n\rNIVEL  Jugador       Nivel  Jugador\n\r", ch );
	  show_file( ch, filename );
	  return;
	}
    }
    else
    {
        send_to_char( "Como?\n\r", ch );
        return;
    }
}


void do_shove( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int exit_dir;
    EXIT_DATA *pexit;
    CHAR_DATA *victim;
    bool nogo;
    ROOM_INDEX_DATA *to_room;
    int chance = 0;
    int race_bonus = 0;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg2 );

    if ( IS_NPC(ch)
    || !IS_SET( ch->pcdata->flags, PCFLAG_DEADLY ) )
    {
   send_to_char("Solo los jugadores PK pueden retarse.\n\r", ch);
	return;
    }

    if  ( get_timer(ch, TIMER_PKILLED) > 0)
    {
   send_to_char("No puedes retar a unjugador justo ahora.\n\r", ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
   send_to_char( "Retar a quien?\n\r", ch);
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
   send_to_char( "Pero si no esta aqui!\n\r", ch);
	return;
    }

    if (victim == ch)
    {
   send_to_char("Te retas a ti mismo, buscas la perfeccion?.\n\r", ch);
	return;
    }
    if ( IS_NPC(victim)
    || !IS_SET( victim->pcdata->flags, PCFLAG_DEADLY ) )
    {
   send_to_char("Solamente puedes retar a jugadores PK.\n\r", ch);
	return;
    }

    if ( ch->level - victim->level > 20
    ||   victim->level - ch->level > 20 )
    {
   send_to_char("Hay demasiada diferencia de nivel entre vosotros para molestarse.\n\r", ch);
	return;
    }

    if  ( get_timer(victim, TIMER_PKILLED) > 0)
    {
   send_to_char("No puedes retar a ese jugador justo ahora.\n\r", ch);
	return;
    }

    if ( (victim->position) != POS_STANDING )
    {
   act( AT_PLAIN, "$N no esta en pie.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( arg2[0] == '\0' )
    {
   send_to_char( "Retarlo en que direccion?\n\r", ch);
	return;
    }

    exit_dir = get_dir( arg2 );
    if ( IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    &&  get_timer(victim, TIMER_SHOVEDRAG) <= 0)
    {
   send_to_char("Este jugador no puede ser retado justo ahora.\n\r", ch);
	return;
    }
    victim->position = POS_SHOVE;
    nogo = FALSE;
    if ((pexit = get_exit(ch->in_room, exit_dir)) == NULL )
      nogo = TRUE;
    else
    if ( IS_SET(pexit->exit_info, EX_CLOSED)
    && (!IS_AFFECTED(victim, AFF_PASS_DOOR ||
	!xIS_SET(ch->afectado_por, DAF_INCORPOREO))
    ||   IS_SET(pexit->exit_info, EX_NOPASSDOOR)) )
      nogo = TRUE;
    if ( nogo )
    {
   send_to_char( "No hay ninguna salida en aquella direccion.\n\r", ch );
        victim->position = POS_STANDING;
	return;
    }
    to_room = pexit->to_room;
    if (IS_SET(to_room->room_flags, ROOM_DEATH))
    {
      send_to_char("No puedes retar a alguien dentro de una trampa mortal.\n\r", ch);
      victim->position = POS_STANDING;
      return;
    }

    if (ch->in_room->area != to_room->area
    &&  !in_hard_range( victim, to_room->area ) )
    {
      send_to_char("Ese personaje no puede entrar en el area.\n\r", ch);
      victim->position = POS_STANDING;
      return;
    }

/* Check for class, assign percentage based on that. */
if (ch->class == CLASS_WARRIOR)
  chance = 70;
if (ch->class == CLASS_VAMPIRE)
  chance = 65;
if (ch->class == CLASS_RANGER)
  chance = 60;
if (ch->class == CLASS_DRUID)
  chance = 45;
if (ch->class == CLASS_CLERIC)
  chance = 35;
if (ch->class == CLASS_THIEF)
  chance = 30;
if (ch->class == CLASS_MAGE)
  chance = 15;

/* Add 3 points to chance for every str point above 15, subtract for
below 15 */

chance += ((get_curr_str(ch) - 15) * 3);

chance += (ch->level - victim->level);

if (ch->race == 1)
race_bonus = -3;

if (ch->race == 2)
race_bonus = 3;

if (ch->race == 3)
race_bonus = -5;

if (ch->race == 4)
race_bonus = -7;

if (ch->race == 6)
race_bonus = 5;

if (ch->race == 7)
race_bonus = 7;

if (ch->race == 8)
race_bonus = 10;

if (ch->race == 9)
race_bonus = -2;

chance += race_bonus;

/* Debugging purposes - show percentage for testing */

/* sprintf(buf, "Shove percentage of %s = %d", ch->name, chance);
act( AT_ACTION, buf, ch, NULL, NULL, TO_ROOM );
*/

if (chance < number_percent( ))
{
  send_to_char("Fallaste.\n\r", ch);
  victim->position = POS_STANDING;
  return;
}
    act( AT_ACTION, "You shove $M.", ch, NULL, victim, TO_CHAR );
    act( AT_ACTION, "$n shoves you.", ch, NULL, victim, TO_VICT );
    move_char( victim, get_exit(ch->in_room,exit_dir), 0);
    if ( !char_died(victim) )
      victim->position = POS_STANDING;
    if (!IS_IMMORTAL( ch ))
	    WAIT_STATE(ch, 12);
    /* Remove protection from shove/drag if char shoves -- Blodkai */
    if ( IS_SET(ch->in_room->room_flags, ROOM_SAFE)
    &&   get_timer(ch, TIMER_SHOVEDRAG) <= 0 )
      add_timer( ch, TIMER_SHOVEDRAG, 10, NULL, 0 );
}

void do_drag( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int exit_dir;
    CHAR_DATA *victim;
    EXIT_DATA *pexit;
    ROOM_INDEX_DATA *to_room;
    bool nogo;
    int chance = 0;
    int race_bonus = 0;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg2 );

    if ( IS_NPC(ch) )
    /* || !IS_SET( ch->pcdata->flags, PCFLAG_DEADLY ) )  */
    {
   send_to_char("Solo los juagdores pueden arrastrar.\n\r", ch);
	return;
    }

    if  ( get_timer(ch, TIMER_PKILLED) > 0)
    {
   send_to_char("No puedes arrastrar a ese jugador ahora mismo.\n\r", ch);
	return;
    }

    if ( arg[0] == '\0' )
    {
   send_to_char( "Arrastrar a quien?\n\r", ch);
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
   send_to_char( "No esta aqui.\n\r", ch);
	return;
    }

    if ( victim == ch )
    {
   send_to_char("Te pasas el brazo alrededor del cuello pero no te arrastras hacia ninguna parte.\n\r", ch);
	return;
    }

    if ( IS_NPC(victim)  )
         /* || !IS_SET( victim->pcdata->flags, PCFLAG_DEADLY ) ) */
    {
   send_to_char("Solo puedes arrastrar a Jugadores.\n\r", ch);
	return;
    }

    if ( !xIS_SET( victim->act, PLR_SHOVEDRAG)
    &&   !IS_SET( victim->pcdata->flags, PCFLAG_DEADLY) )
    {
   send_to_char("Este Jugador no parace apreciar tu atraccion\n\r", ch);
	return;
    }

    if  ( get_timer(victim, TIMER_PKILLED) > 0)
    {
   send_to_char("No puedes arrastrar a este Jugador ahora mismo.\n\r", ch);
	return;
    }

    if ( victim->fighting )
    {
        send_to_char( "Lo intentas pero no consigues situarte bastante cerca.\n\r", ch);
        return;
    }

    if( !IS_SET(ch->pcdata->flags, PCFLAG_DEADLY ) && IS_SET( victim->pcdata->flags, PCFLAG_DEADLY ) ){
   send_to_char("No puedes arrastrar a un Jugador PK.\n\r", ch);
	return;
    }

    if ( !IS_SET(victim->pcdata->flags, PCFLAG_DEADLY) && victim->position > 3 )
    {
   send_to_char("No parece que necesite tu ayuda.\n\r", ch );
	return;
    }

    if ( arg2[0] == '\0' )
    {
   send_to_char( "Arrastralo hacia que dreccion?\n\r", ch);
	return;
    }

    if ( ch->level - victim->level > 5
    ||   victim->level - ch->level > 5 )
    {
	if ( IS_SET(victim->pcdata->flags, PCFLAG_DEADLY)
	&&   IS_SET(ch->pcdata->flags, PCFLAG_DEADLY) )
	{
          send_to_char("Demasiada diferencia de niveles para que te molestes.\n\r", ch);
          return;
	}
    }

    exit_dir = get_dir( arg2 );

    if ( IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    &&   get_timer( victim, TIMER_SHOVEDRAG ) <= 0)
    {
   send_to_char("Este Jugador no puede ser arrastrado ahora mismo.\n\r", ch);
	return;
    }

    nogo = FALSE;
    if ((pexit = get_exit(ch->in_room, exit_dir)) == NULL )
      nogo = TRUE;
    else
    if ( IS_SET(pexit->exit_info, EX_CLOSED)
    && (!IS_AFFECTED(victim, AFF_PASS_DOOR || 
	!xIS_SET(ch->afectado_por, DAF_INCORPOREO))
    ||   IS_SET(pexit->exit_info, EX_NOPASSDOOR)) )
      nogo = TRUE;
    if ( nogo )
    {
   send_to_char( "No hay salida en esa dreccion.\n\r", ch );
	return;
    }

    to_room = pexit->to_room;
    if (IS_SET(to_room->room_flags, ROOM_DEATH))
    {
      send_to_char("No puedes arrastras a nadie dentro de una trampa mortal.\n\r", ch);
      return;
    }

    if (ch->in_room->area != to_room->area
    && !in_hard_range( victim, to_room->area ) )
    {
      send_to_char("Este Jugador no puede entrar dentro del area.\n\r", ch);
      victim->position = POS_STANDING;
      return;
    }

/* Check for class, assign percentage based on that. */
if (ch->class == CLASS_WARRIOR)
  chance = 70;
if (ch->class == CLASS_VAMPIRE)
  chance = 65;
if (ch->class == CLASS_RANGER)
  chance = 60;
if (ch->class == CLASS_DRUID)
  chance = 45;
if (ch->class == CLASS_CLERIC)
  chance = 35;
if (ch->class == CLASS_THIEF)
  chance = 30;
if (ch->class == CLASS_MAGE)
  chance = 15;

/* Add 3 points to chance for every str point above 15, subtract for
below 15 */

chance += ((get_curr_str(ch) - 15) * 3);

chance += (ch->level - victim->level);

if (ch->race == 1)
race_bonus = -3;

if (ch->race == 2)
race_bonus = 3;

if (ch->race == 3)
race_bonus = -5;

if (ch->race == 4)
race_bonus = -7;

if (ch->race == 6)
race_bonus = 5;

if (ch->race == 7)
race_bonus = 7;

if (ch->race == 8)
race_bonus = 10;

if (ch->race == 9)
race_bonus = -2;

chance += race_bonus;
/*
sprintf(buf, "Drag percentage of %s = %d", ch->name, chance);
act( AT_ACTION, buf, ch, NULL, NULL, TO_ROOM );
*/
if (chance < number_percent( ))
{
  send_to_char("Fallaste.\n\r", ch);
  victim->position = POS_STANDING;
  return;
}
    if ( victim->position < POS_STANDING )
    {
	sh_int temp;

	temp = victim->position;
	victim->position = POS_DRAG;
   act( AT_ACTION, "Arrastras a $M desntro de la habitacion siguiente.", ch, NULL, victim, TO_CHAR );
   act( AT_ACTION, "$n agarra tu pelo y te arrastra.", ch, NULL, victim, TO_VICT );
	move_char( victim, get_exit(ch->in_room,exit_dir), 0);
	if ( !char_died(victim) )
	  victim->position = temp;
/* Move ch to the room too.. they are doing dragging - Scryn */
	move_char( ch, get_exit(ch->in_room,exit_dir), 0);
	WAIT_STATE(ch, 12);
	return;
    }
    send_to_char("No no puedes hacer eso a alguien que esta de pie .\n\r", ch);
    return;
}

void do_pertenecer( CHAR_DATA *ch, char *argument, RACE_TYPE *raza )
{
        char buf[MAX_STRING_LENGTH];

        if ( argument[0] == '\0' )
        {
                if ( IS_VAMPIRE(ch) )
                  send_to_char( "&wSintaxis:        pertenecer <camarilla | sabbath | independiente>\n\r", ch );     return;
                if ( ES_CRUZADO(ch) )
                  send_to_char( "&wSintaxis:        pertenecer <satanico | celestial>\n\r", ch );     return;
                if ( ES_MAGO(ch) )
                  send_to_char( "&wSintaxis:        pertenecer <eutanatos | nefando>\n\r", ch );     return;
                if ( ES_HADA(ch) )
                  send_to_char( "&wSintaxis:        pertenecer <sidhe | pooka>\n\r", ch );     return;
                if ( ES_LICANTROPO(ch) )
                  send_to_char( "&wSintaxis:        pertenecer <uktena | fenrir>\n\r", ch );     return;
                if ( ch->class == 2 )
                  send_to_char( "&wSintaxis:        pertenecer <mafia | asesinos | cazarecompensas>\n\r", ch );     return;
           return;
         }

       if ( IS_VAMPIRE(ch) )
       {
                if ( !str_prefix( argument,  "camarilla" ) && ch->race != 2)
                {
                send_to_char( "Pasas a ser miembro de la Camarilla. Ayuda 'CAMARILLA'.\n\r", ch );
                sprintf( buf, "%s pasa a formar parte de la Camarilla!\n\r", ch->name );
                echo_to_all( AT_RED , buf, ECHOTAR_ALL );
                ch->race = 2;
                return;
                }
                else if ( !str_prefix( argument,  "camarilla" ) && ch->race == 2)
                send_to_char( "Ya perteneces a la camarilla.\n\r", ch );
                if ( !str_prefix( argument,  "sabbath" ) && ch->race != 1)
                {
                send_to_char( "Pasas a ser miembro del Sabbath. Ayuda 'SABBATH'.\n\r", ch );
                sprintf( buf, "%s pasa a formar parte del Sabbath!\n\r", ch->name );
                echo_to_all( AT_RED , buf, ECHOTAR_ALL );
                ch->race = 1;
                return;
                }
                else if ( !str_prefix( argument,  "sabbath" ) && ch->race == 1)
                send_to_char( "Ya perteneces al Sabbath.\n\r", ch );

                if ( !str_prefix( argument,   "independiente" ) && ch->race != 3)
                {
                send_to_char( "Pasas a ser Independiente. Ayuda 'INDEPENDENCIA'.\n\r", ch );
                sprintf( buf, "%s pasa a ser Independiente!\n\r", ch->name );
                echo_to_all( AT_RED , buf, ECHOTAR_ALL );
                ch->race = 3;
                return;
                }
                else if ( !str_prefix( argument,   "independiente" ) && ch->race == 3)
                send_to_char( "Ya eres Independiente.\n\r", ch );
         }

         if ( ES_CRUZADO(ch) )
         {
                if ( !str_prefix( argument,   "satanicos" ) && ch->race != 11)
                {
                send_to_char( "Pasas a ser miembro de los Cruzados Satanicos. Ayuda 'SATANICOS'.\n\r", ch );
                sprintf( buf, "%s pasa a formar parte de los Cruzados Satanicos!\n\r", ch->name );
                echo_to_all( AT_RED , buf, ECHOTAR_ALL );
                ch->race = 11;
                return;
                }
                else if ( !str_prefix( argument,   "satanicos" ) && ch->race == 11)
                send_to_char( "Ya perteneces a los Cruzados Satanicos.\n\r", ch );

                if ( !str_prefix( argument,   "celestial" ) && ch->race != 10)
                {
                send_to_char( "Pasas a ser miembro de los Cruzados Celestiales. Ayuda 'CELESTIALES'.\n\r", ch );
                sprintf( buf, "%s pasa a formar parte de los Cruzados Celestiales!\n\r", ch->name );
                echo_to_all( AT_RED , buf, ECHOTAR_ALL );
                ch->race = 10;
                return;
                }
                else if ( !str_prefix( argument,   "celestial" ) && ch->race == 10)
                send_to_char( "Ya perteneces a los Cruzados Celestiales.\n\r", ch );
           }

           if ( ES_MAGO( ch ))
           {
                if ( !str_prefix( argument,   "nefandos" ) && ch->race != 7)
                {
                send_to_char( "Pasas a ser miembro de los Nefandos. Ayuda 'NEFANDOS'.\n\r", ch );
                sprintf( buf, "%s pasa a formar parte de los Nefandos!\n\r", ch->name );
                echo_to_all( AT_RED , buf, ECHOTAR_ALL );
                ch->race = 7;
                return;
                }
                else if ( !str_prefix( argument,   "nefandos" ) && ch->race == 7)
                send_to_char( "Ya perteneces a los Nefandos.\n\r", ch );

                if ( !str_prefix( argument,   "autanatos" ) && ch->race != 6)
                {
                send_to_char( "Pasas a ser miembro de los Eutanatos. Ayuda 'EUTANATOS'.\n\r", ch );
                sprintf( buf, "%s pasa a formar parte de los Eutanatos!\n\r", ch->name );
                echo_to_all( AT_RED , buf, ECHOTAR_ALL );
                ch->race = 6;
                return;
                }
                else if ( !str_prefix( argument,   "eutanatos" ) && ch->race == 6)
                send_to_char( "Ya perteneces a los Eutanatos.\n\r", ch );
              }

              if ( ES_HADA( ch ) )
              {
                        if ( !str_prefix( argument,   "sidhe" ) && ch->race != 5)
                {
                send_to_char( "Pasas a ser miembro de los Guerreros Sidhe. Ayuda 'SIDHE'.\n\r", ch );
                sprintf( buf, "%s pasa a formar parte de los Guerreros Sidhe!\n\r", ch->name );
                echo_to_all( AT_RED , buf, ECHOTAR_ALL );
                ch->race = 5;
                return;
                }
                else if ( !str_prefix( argument,   "sidhe" ) && ch->race == 5)
                send_to_char( "Ya perteneces a los Guerreros Sidhe.\n\r", ch );

                if ( !str_prefix( argument,   "pooka" ) && ch->race != 4)
                {
                send_to_char( "Pasas a ser miembro de los Guerreros Pooka. Ayuda 'POOKA'.\n\r", ch );
                sprintf( buf, "%s pasa a formar parte de los Guerreos Pooka!\n\r", ch->name );
                echo_to_all( AT_RED , buf, ECHOTAR_ALL );
                ch->race = 4;
                return;
                }
                else if ( !str_prefix( argument,   "pooka" ) && ch->race == 4)
                send_to_char( "Ya perteneces a los Guerreros Pooka.\n\r", ch );
              }

             if ( ch->class == 2 )
             {
                send_to_char( "Lo sentimos aun estamos implementando tu procedencia.\n\r", ch );
                return;
             }
             else
             {
                send_to_char( "Eso no es una procedencia, prueba otra.\n\r", ch );
                return;
             }

             return;
             }

/* Ascender Degradar y Abdiscar por COLIKOTRON code team */
void do_ascender( CHAR_DATA *pj, char *argument, CLAN_DATA *familia  )
{
        char buf[MAX_STRING_LENGTH];
        char buf2[MAX_STRING_LENGTH];

        CHAR_DATA *ascendido;

        if ( pj->pcdata->clan == NULL )
        {
                send_to_char( "No estas en ninguna familia.\n\r", pj );
                return;
        }

        if ( pj->pcdata->clan->leader != pj->name )
        {
                send_to_char( "Tu no eres el lider de la familia.\n\r", pj );
                return;
        }

        if ( argument[0] == '\0' )
        {
                send_to_char( "&wA quien quieres ascender?\n\r", pj );
                return;
        }

     if ( ( ascendido = get_char_room( pj, argument ) ) == NULL )
    {
             send_to_char( "Ese jugador no esta aqui.\n\r", pj);
     return;
     }

     if ( IS_NPC( ascendido ) )
     {
        send_to_char( "A un mob?\n\r", pj );
        return;
     }

     if ( IS_NPC( pj ) )
     {
        send_to_char( "Los Mobs no pueden ascender a nadie.\n\r", pj );
        return;
     }

        if ( pj->pcdata->clan != ascendido->pcdata->clan )
        {
                send_to_char( "Pero si ese no es de tu familia!\n\r", pj );
                return;
        }

        if ( ascendido->pcdata->clan->number1 == ascendido->name )
        {
                send_to_char( "Deberas abdicar para que el pueda ascender de rango.\n\r", pj );
                return;
        }

        if ( ascendido == pj )
        {
                send_to_char( "Que quieres ser Dios o que?\n\r", pj );
                return;
        }

        if ( get_trust( ascendido ) >= LEVEL_IMMORTAL )
        {
                send_to_char( "A un inmortal.... mejor no he.\n\r", pj );
                return;
        }

        if ( ascendido->pcdata->clan->number2 == ascendido->name )
        {
        if ( ascendido->pcdata->clan->number1 != NULL )
                {
                        sprintf( buf2, "&o%s&g &gha sido nombrado Consejero de la familia %s.\n\r", ascendido->pcdata->clan->number1, ascendido->pcdata->clan->name );
                        echo_to_all( AT_ORANGE, buf2, ECHOTAR_ALL );
                }

                ch_printf( pj, "&bAsciendes a &w%s &ben la Familia %s a rango de Heredero!\n\r", ascendido->name, ascendido->pcdata->clan->name );
                sprintf( buf, "&o%s &gha sido nombrado heredero de la familia %s.\n\r", ascendido->name, ascendido->pcdata->clan->name );
                echo_to_all( AT_ORANGE, buf, ECHOTAR_ALL );
                ascendido->pcdata->clan->number2 = ascendido->pcdata->clan->number1;
                ascendido->pcdata->clan->number1 = ascendido->name;
                save_clan( pj->pcdata->clan );
                return;
         }

        if ( ascendido->pcdata->clan->name == pj->pcdata->clan->name
              && pj->pcdata->clan->number1 != ascendido->name ||  pj->pcdata->clan->number2 != ascendido->name)
                      {
                      if ( ascendido->pcdata->clan->number2 != NULL )
                {
                        sprintf( buf2, "&o%s&g &gha sido nombrado Miembro de la familia %s.\n\r", ascendido->pcdata->clan->number2, ascendido->pcdata->clan->name );
                        echo_to_all( AT_ORANGE, buf2, ECHOTAR_ALL );
                }

                ch_printf( pj, "&bAsciendes a &w%s &ben la Familia %s a rango de Consejero!\n\r", ascendido->name, ascendido->pcdata->clan->name );
                sprintf( buf, "&o%s &gha sido nombrado Consejero de la familia %s.\n\r", ascendido->name, ascendido->pcdata->clan->name );
                echo_to_all( AT_ORANGE, buf, ECHOTAR_ALL );
                ascendido->pcdata->clan->number2 = ascendido->name;
                save_clan( pj->pcdata->clan );
                return;
         }
         return;
}


void do_degradar( CHAR_DATA *pj, char *argument, CLAN_DATA *familia  )
{
        char buf[MAX_STRING_LENGTH];
        char buf2[MAX_STRING_LENGTH];

        CHAR_DATA *degradado;

        if ( pj->pcdata->clan == NULL )
        {
                send_to_char( "No estas en ninguna familia.\n\r", pj );
                return;
        }

        if ( pj->pcdata->clan->leader != pj->name )
        {
                send_to_char( "Tu no eres el lider de la familia.\n\r", pj );
                return;
        }

        if ( argument[0] == '\0' )
        {
                send_to_char( "&wA quien quieres degradar?\n\r", pj );
                return;
        }

     if ( ( degradado = get_char_room( pj, argument ) ) == NULL )
    {
             send_to_char( "Ese jugador no esta aqui.\n\r", pj);
     return;
     }

     if ( IS_NPC( degradado ) )
     {
        send_to_char( "A un mob?\n\r", pj );
        return;
     }

     if ( IS_NPC( pj ) )
     {
        send_to_char( "Los Mobs no pueden ascender a nadie.\n\r", pj );
        return;
     }

        if ( pj->pcdata->clan != degradado->pcdata->clan )
        {
                send_to_char( "Pero si ese no es de tu familia!\n\r", pj );
                return;
        }
        /*
        * Ke alguien no explike por ke esto no va...

        if (  degradado->pcdata->clan->number1 != degradado->name
        || degradado->pcdata->clan->number2 != degradado->name )
        {
                send_to_char( "A ese expulsalo en todo caso.\n\r", pj );
                return;
        }
        */
        if ( degradado == pj )
        {
                send_to_char( "Si quieres eso deberas de Abdicar?\n\r", pj );
                return;
        }

        if ( get_trust( degradado ) >= LEVEL_IMMORTAL )
        {
                send_to_char( "A un inmortal.... mejor no he.\n\r", pj );
                return;
        }

        if ( degradado->pcdata->clan->number1 == degradado->name )
        {
        if ( degradado->pcdata->clan->number2 != NULL )
                {
                        sprintf( buf2, "&o%s&g &gha sido ascendido a Heredero de la familia %s.\n\r", degradado->pcdata->clan->number2, degradado->pcdata->clan->name );
                        echo_to_all( AT_ORANGE, buf2, ECHOTAR_ALL );
                }

                ch_printf( pj, "&bDegradas  a &w%s &ben la Familia %s a rango de Consejero!\n\r", degradado->name, degradado->pcdata->clan->name );
                sprintf( buf, "&o%s &gha sido nombrado Consejero de la familia %s.\n\r", degradado->name, degradado->pcdata->clan->name );
                echo_to_all( AT_ORANGE, buf, ECHOTAR_ALL );
                degradado->pcdata->clan->number1 = degradado->pcdata->clan->number2;
                degradado->pcdata->clan->number2 = degradado->name;
                save_clan( pj->pcdata->clan );
                return;
         }

       if ( degradado->pcdata->clan->number2 == degradado->name )
       {
                        sprintf( buf2, "&o%s&g &gha sido nombrado Miembro de la familia %s.\n\r", degradado->name, degradado->pcdata->clan->name );
                        echo_to_all( AT_ORANGE, buf2, ECHOTAR_ALL );
                        sprintf( buf2, "&g &gQueda vacante el puesto de Consejero en la familia %s.\n\r",  degradado->pcdata->clan->name );
                        echo_to_all( AT_GREY, buf2, ECHOTAR_ALL );

                ch_printf( pj, "&bDegradas a &w%s &ben la Familia %s a rango de Miembro!\n\r", degradado->name, degradado->pcdata->clan->name );
                degradado->pcdata->clan->number2 = "VACANTE";
                save_clan( pj->pcdata->clan );
                return;
         }
         return;
}


void do_abdicar( CHAR_DATA *pj, char *argument, CLAN_DATA *familia  )
{
        char buf[MAX_STRING_LENGTH];

        CHAR_DATA *ascendido;

        if ( pj->pcdata->clan == NULL )
        {
                send_to_char( "No estas en ninguna familia.\n\r", pj );
                return;
        }

        if ( pj->pcdata->clan->leader != pj->name )
        {
                send_to_char( "Tu no eres el lider de la familia.\n\r", pj );
                return;
        }

        if ( argument[0] == '\0' )
        {
                send_to_char( "&wPor quien quieres abdicar?\n\r", pj );
                return;
        }

     if ( ( ascendido = get_char_room( pj, argument ) ) == NULL )
    {
             send_to_char( "Ese jugador no esta aqui.\n\r", pj);
     return;
     }

     if ( IS_NPC( ascendido ) )
     {
        send_to_char( "A un mob?\n\r", pj );
        return;
     }

     if ( IS_NPC( pj ) )
     {
        send_to_char( "Los Mobs no pueden ascender a nadie.\n\r", pj );
        return;
     }

        if ( pj->pcdata->clan != ascendido->pcdata->clan )
        {
                send_to_char( "Pero si ese no es de tu familia!\n\r", pj );
                return;
        }

        if ( pj->pcdata->clan->number1 != ascendido->name )
        {
                send_to_char( "Debe de ser el Heredero de la Familia.\n\r", pj );
                return;
        }

        if ( ascendido == pj )
        {
                send_to_char( "Tu estas tonto o que?\n\r", pj );
                return;
        }

        if ( get_trust( ascendido ) >= LEVEL_IMMORTAL )
        {
                send_to_char( "A un inmortal.... mejor no he.\n\r", pj );
                return;
        }


        pj->pcdata->clan->leader = ascendido->name;
        pj->pcdata->clan->number1 = pj->name;

        sprintf( buf, "&wHas abdicado relevando tu puesto de lider a %s.\n\r", ascendido->name );
        send_to_char( buf, pj );
        sprintf( buf, "&w%s ha abdicado relevandote el puesto de lider.\n\r", pj->name );
        send_to_char( buf, ascendido );

        sprintf( buf, "%s &gha abdicado a favor de &o%s &gque pasa a ser lider de la familia %s&g!\n\r", pj->name, ascendido->name,  pj->pcdata->clan->name );
        echo_to_all( AT_ORANGE, buf, ECHOTAR_ALL );
        save_clan( pj->pcdata->clan );
        return;
    }


void busca_con_area( CLAN_DATA *familia )
{
  AREA_DATA *pArea;
  CON_DATA *conquista;
  int acon = 0;

  for(pArea = first_area; pArea; pArea->next)
    {
      conquista = get_conquista( pArea->name );
      if( conquista->propietario == familia->name)
	acon++;
    }
  a_con = acon;
  return;
}

void busca_con_pj( CLAN_DATA *familia )
{
  DESCRIPTOR_DATA *d;
  int pcon = 0;

  for( d = first_descriptor; d; d->next)
    {
      if(d->character->pcdata->clan->name == familia->name )
	pcon++;
    }
  p_con = pcon;
  return;
}

void do_miembros( CHAR_DATA *ch, char *argument)
{

	FILE *fichero;
	int i;
	char miembro[256];
	char elclan[256];
	
	if ( IS_NPC( ch ) || !ch->pcdata->clan )
	{
		send_to_char( "Huh?\n\r", ch );
	        return;
	}
	send_to_char("Estos son los miembros de tu Familia:\n\r\n\r", ch);
	sprintf( elclan, "%s%s.txt",CLAN_DIR, ch->pcdata->clan_name);
	fichero = fopen (elclan, "r");
	for (i=0;i<50;i++)
	{
		fscanf(fichero,"%s\n",miembro);
		if (!strcmp(miembro,"~"))
			break;
		send_to_char(miembro,ch);
		send_to_char("\n\r",ch);
		
	}
	fclose(fichero);

}
