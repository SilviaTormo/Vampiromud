        /****************************************************************************
 * [S]imulated [M]edieval [A]dventure multi[U]ser [G]ame      |   \\._.//   *
 * -----------------------------------------------------------|   (0...0)   *
 * SMAUG 1.4 (C) 1994, 1995, 1996, 1998  by Derek Snider      |    ).:.(    *
 * -----------------------------------------------------------|    {o o}    *
 * SMAUG code team: Thoric, Altrag, Blodkai, Narn, Haus,      |   / ' ' \   *
 * Scryn, Rennard, Swordbearer, Gorog, Grishnakh, Nivek,      |~'~.VxvxV.~'~*
 * Tricops and Fireblade                                      |             *
 * ------------------------------------------------------------------------ *
 * Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael        *
 * Chastain, Michael Quan, and Mitchell Tse.                                *
 * Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          *
 * Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     *
 * ------------------------------------------------------------------------ *
 *			     Mud constants module			    *
 ****************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <time.h>
#include "mud.h"



/* undef these at EOF */
#define AM 95
#define AC 95
#define AT 85
#define AW 85
#define AV 95
#define AD 95
#define AR 90
#define AA 95

char *	const	npc_race	[MAX_NPC_RACE] =
{
"humano", "sabbath", "camarilla", "independiente", "pooka", "sidhe", "eutanatos",
"nefando", "uktena", "fenrir", "celestial", "satanico", "elfo-acuatico",
"hombre-lagarto", "gnomo", "r5", "r6", "r7", "r8", "troll",
"hormiga", "simio", "babuino", "muercielago", "oso", "abeja",
"escarabajo", "pesadilla", "perro", "dragon", "huron", "mosca",
"gargola", "gelatina", "ghoul", "gnoll", "gnomo", "goblin", "golem",
"gorgon", "harpia", "hobgoblin", "kobold", "hombre-lagarto", "langosta",
"licantropo", "minotauro", "moho", "mula", "neanderthal", "cieno", "orco",
"rata", "monstruo-planta", "sombra", "engendro", "musaranya", "shrieker",
"esqueleto", "lodo", "serpiente", "aranya", "stirge", "thoul", "troglodita",
"no-muerto", "wight", "lobo", "gusano", "zombi", "bovino", "canino", "felino",
"porcino", "mamifero", "roedor", "anvis", "reptil", "anfibio", "pez",
"crustaceo", "insecto", "espiritu", "magico", "caballo", "animal", "humanoide",
"monstruo", "dios"
};


char *	const	npc_class	[MAX_NPC_CLASS] =
{
"mago", "clerigo", "ladron", "escoria", "assamita", "capadocio", "licantropo",
"cruzado", "hada", "malkavian", "tremere", "baali", "nosferatu", "gangrel",
"toreador", "brujha", "ventrue", "pc17", "pc18", "pc19",
"panadero", "carnicero", "herrero", "alcalde", "rey", "reina"
};



#if 0
const struct at_color_type at_color_table[AT_MAXCOLOR] =
{ { "plain",	AT_GREY			},
  { "action",	AT_GREY			},
  { "say",	AT_LBLUE		},
  { "gossip",	AT_LBLUE		},
  { "yell",	AT_WHITE		},
  { "tell",	AT_WHITE		},
  { "whisper",	AT_WHITE		},
  { "hit",	AT_WHITE		},
  { "hitme",	AT_YELLOW		},
  { "immortal",	AT_YELLOW		},
  { "hurt",	AT_RED			},
  { "falling",	AT_WHITE + AT_BLINK	},
  { "danger",	AT_RED + AT_BLINK	},
  { "magic",	AT_BLUE			},
  { "consider",	AT_GREY			},
  { "report",	AT_GREY			},
  { "poison",	AT_GREEN		},
  { "social",	AT_CYAN			},
  { "dying",	AT_YELLOW		},
  { "dead",	AT_RED			},
  { "skill",	AT_GREEN		},
  { "carnage",	AT_BLOOD		},
  { "damage",	AT_WHITE		},
  { "flee",	AT_YELLOW		},
  { "roomname",	AT_WHITE		},
  { "roomdesc",	AT_YELLOW		},
  { "object",	AT_GREEN		},
  { "person",	AT_PINK			},
  { "list",	AT_BLUE			},
  { "bye",	AT_GREEN		},
  { "gold",	AT_YELLOW		},
  { "gtell",	AT_BLUE			},
  { "note",	AT_GREEN		},
  { "hungry",	AT_ORANGE		},
  { "thirsty",	AT_BLUE			},
  { "fire",	AT_RED			},
  { "sober",	AT_WHITE		},
  { "wearoff",	AT_YELLOW		},
  { "exits",	AT_WHITE		},
  { "score",	AT_LBLUE		},
  { "reset",	AT_DGREEN		},
  { "log",	AT_PURPLE		},
  { "diemsg",	AT_WHITE		},
  { "wartalk",	AT_RED			},
  { "racetalk",	AT_DGREEN		},
  { "ignore",	AT_GREEN		},
  { "divider",	AT_PLAIN		},
  { "morph",	AT_GREY			},
};
#endif


/*
 * Attribute bonus tables.
 */
const	struct	str_app_type	str_app		[26]		=
{
    { -5, -4,   0,  0 },  /* 0  */
    { -5, -4,   3,  1 },  /* 1  */
    { -3, -2,   3,  2 },
    { -3, -1,  10,  3 },  /* 3  */
    { -2, -1,  25,  4 },
    { -2, -1,  55,  5 },  /* 5  */
    { -1,  0,  80,  6 },
    { -1,  0,  90,  7 },
    {  0,  0, 100,  8 },
    {  0,  0, 100,  9 },
    {  0,  0, 115, 10 }, /* 10  */
    {  0,  0, 115, 11 },
    {  0,  0, 140, 12 },
    {  0,  0, 140, 13 }, /* 13  */
    {  0,  1, 170, 14 },
    {  1,  1, 170, 15 }, /* 15  */
    {  1,  2, 195, 16 },
    {  2,  3, 220, 22 },
    {  2,  4, 250, 25 }, /* 18  */
    {  3,  5, 400, 30 },
    {  3,  6, 500, 35 }, /* 20  */
    {  4,  7, 600, 40 },
    {  5,  7, 700, 45 },
    {  6,  8, 800, 50 },
    {  8, 10, 900, 55 },
    { 10, 12, 999, 60 }  /* 25   */
};



const	struct	int_app_type	int_app		[26]		=
{
    {  3 },	/*  0 */
    {  5 },	/*  1 */
    {  7 },
    {  8 },	/*  3 */
    {  9 },
    { 10 },	/*  5 */
    { 11 },
    { 12 },
    { 13 },
    { 15 },
    { 17 },	/* 10 */
    { 19 },
    { 22 },
    { 25 },
    { 28 },
    { 31 },	/* 15 */
    { 34 },
    { 37 },
    { 40 },	/* 18 */
    { 44 },
    { 49 },	/* 20 */
    { 55 },
    { 60 },
    { 70 },
    { 85 },
    { 99 }	/* 25 */
};



const	struct	wis_app_type	wis_app		[26]		=
{
    { 0 },	/*  0 */
    { 0 },	/*  1 */
    { 0 },
    { 0 },	/*  3 */
    { 0 },
    { 1 },	/*  5 */
    { 1 },
    { 1 },
    { 1 },
    { 2 },
    { 2 },	/* 10 */
    { 2 },
    { 2 },
    { 2 },
    { 2 },
    { 3 },	/* 15 */
    { 3 },
    { 4 },
    { 5 },	/* 18 */
    { 5 },
    { 5 },	/* 20 */
    { 6 },
    { 6 },
    { 6 },
    { 6 },
    { 7 }	/* 25 */
};



const	struct	dex_app_type	dex_app		[26]		=
{
    {   60 },   /* 0 */
    {   50 },   /* 1 */
    {   50 },
    {   40 },
    {   30 },
    {   20 },   /* 5 */
    {   10 },
    {    0 },
    {    0 },
    {    0 },
    {    0 },   /* 10 */
    {    0 },
    {    0 },
    {    0 },
    {    0 },
    { - 10 },   /* 15 */
    { - 15 },
    { - 20 },
    { - 30 },
    { - 40 },
    { - 50 },   /* 20 */
    { - 60 },
    { - 75 },
    { - 90 },
    { -105 },
    { -120 }    /* 25 */
};



const	struct	con_app_type	con_app		[26]		=
{
    { -4, 20 },   /*  0 */
    { -3, 25 },   /*  1 */
    { -2, 30 },
    { -2, 35 },	  /*  3 */
    { -1, 40 },
    { -1, 45 },   /*  5 */
    { -1, 50 },
    {  0, 55 },
    {  0, 60 },
    {  0, 65 },
    {  0, 70 },   /* 10 */
    {  0, 75 },
    {  0, 80 },
    {  0, 85 },
    {  0, 88 },
    {  1, 90 },   /* 15 */
    {  2, 95 },
    {  2, 97 },
    {  3, 99 },   /* 18 */
    {  3, 99 },
    {  4, 99 },   /* 20 */
    {  4, 99 },
    {  5, 99 },
    {  6, 99 },
    {  7, 99 },
    {  8, 99 }    /* 25 */
};


const	struct	cha_app_type	cha_app		[26]		=
{
    { - 60 },   /* 0 */
    { - 50 },   /* 1 */
    { - 50 },
    { - 40 },
    { - 30 },
    { - 20 },   /* 5 */
    { - 10 },
    { -  5 },
    { -  1 },
    {    0 },
    {    0 },   /* 10 */
    {    0 },
    {    0 },
    {    0 },
    {    1 },
    {    5 },   /* 15 */
    {   10 },
    {   20 },
    {   30 },
    {   40 },
    {   50 },   /* 20 */
    {   60 },
    {   70 },
    {   80 },
    {   90 },
    {   99 }    /* 25 */
};

/* Have to fix this up - not exactly sure how it works (Scryn) */
const	struct	lck_app_type	lck_app		[26]		=
{
    {   60 },   /* 0 */
    {   50 },   /* 1 */
    {   50 },
    {   40 },
    {   30 },
    {   20 },   /* 5 */
    {   10 },
    {    0 },
    {    0 },
    {    0 },
    {    0 },   /* 10 */
    {    0 },
    {    0 },
    {    0 },
    {    0 },
    { - 10 },   /* 15 */
    { - 15 },
    { - 20 },
    { - 30 },
    { - 40 },
    { - 50 },   /* 20 */
    { - 60 },
    { - 75 },
    { - 90 },
    { -105 },
    { -120 }    /* 25 */
};


/*
 * Liquid properties.
 * Used in #OBJECT section of area file.
 */
const	struct	liq_type	liq_table	[LIQ_MAX]	=
{
    { "agua",			"clear",	{  0, 1, 10 }	},  /*  0 */
    { "cerveza",		"amber",	{  3, 2,  5 }	},
    { "vino",			"rose",		{  5, 2,  5 }	},
    { "ale",			"brown",	{  2, 2,  5 }	},
    { "dark ale",		"dark",		{  1, 2,  5 }	},

    { "whisky",			"golden",	{  6, 1,  4 }	},  /*  5 */
    { "lemonade",		"pink",		{  0, 1,  8 }	},
    { "firebreather",		"boiling",	{ 10, 0,  0 }	},
    { "local specialty",	"everclear",	{  3, 3,  3 }	},
    { "slime mold juice",	"green",	{  0, 4, -8 }	},

    { "milk",			"white",	{  0, 3,  6 }	},  /* 10 */
    { "tea",			"tan",		{  0, 1,  6 }	},
    { "coffee",			"black",	{  0, 1,  6 }	},
    { "blood",			"red",		{  0, 2, -1 }	},
    { "salt water",		"clear",	{  0, 1, -2 }	},

    { "cola",			"cherry",	{  0, 1,  5 }	},  /* 15 */
    { "mead",			"honey color",	{  4, 2,  5 }	},  /* 16 */
    { "grog",			"thick brown",	{  3, 2,  5 }	}   /* 17 */
};

char *	const	attack_table	[19] =
{
    "golpe",
    "tajo",  "punyalada",    "tajo", "latigazo",  "garra",
    "impacto",  "golpe",   "golpe", "impacto",  "mordisco",
    "corte", "succion", "impacto",  "impacto", "impacto",
    "piedra",  "golpe", "disciplina", "critico"
};

char * s_blade_messages[24] =
{
	"falla", "roza", "roza", "aranya", "aranya", "corta", "corta",
	"golpea", "golpea", "raja", "raja", "hiere", "hiere", "demuele",
   "demuele", "DEVASTA", "&PMUTILA&w", "&PDESTROZA&w", "&rDESTRIPA&w",
   "&rDESFIGURA&w", "&y** &RDESCUARTIZA &y**&w", "&B** &REXTERMINA &B**&w", "&G*** &RANNIQUILA &G***&w",
   "&W<<< &RDeJa En EsTaDo CriTiCo &W>>>&w"
};

char * p_blade_messages[24] =
{
	"falla", "roza", "roza", "aranya", "aranya", "corta", "corta",
	"golpea", "golpea", "raja", "raja", "hiere", "hiere", "demuele",
   "demuele", "DEVASTA", "&PMUTILA&w", "&PDESTROZA&w", "&rDESTRIPA&w",
   "&rDESFIGURA&w", "&y** &RDESCUARTIZA &y**&w", "&B** &REXTERMINA &B**&w", "&G*** &RANNIQUILA &G***&w",
   "&W<<< &RDeJa En EsTaDo CriTiCo &W>>>&w"

};

char * s_blunt_messages[24] =
{
	"falla", "roza", "roza", "aranya", "aranya", "corta", "corta",
	"golpea", "golpea", "raja", "raja", "hiere", "hiere", "demuele",
   "demuele", "DEVASTA", "&PMUTILA&w", "&PDESTROZA&w", "&rDESTRIPA&w",
   "&rDESFIGURA&w", "&y** &RDESCUARTIZA &y**&w", "&B** &REXTERMINA &B**&w", "&G*** &RANNIQUILA &G***&w",
   "&W<<< &RDeJa En EsTaDo CriTiCo &W>>>&w"

};

char * p_blunt_messages[24] =
{
	"falla", "roza", "roza", "aranya", "aranya", "corta", "corta",
	"golpea", "golpea", "raja", "raja", "hiere", "hiere", "demuele",
   "demuele", "DEVASTA", "&PMUTILA&w", "&PDESTROZA&w", "&rDESTRIPA&w",
   "&rDESFIGURA&w", "&y** &RDESCUARTIZA &y**&w", "&B** &REXTERMINA &B**&w", "&G*** &RANNIQUILA &G***&w",
   "&W<<< &RDeJa En EsTaDo CriTiCo &W>>>&w"
};

char * s_generic_messages[24] =
{
	"falla", "roza", "roza", "aranya", "aranya", "corta", "corta",
	"golpea", "golpea", "raja", "raja", "hiere", "hiere", "demuele",
   "demuele", "DEVASTA", "&PMUTILA&w", "&PDESTROZA&w", "&rDESTRIPA&w",
   "&rDESFIGURA&w", "&y** &RDESCUARTIZA &y**&w", "&B** &REXTERMINA &B**&w", "&G*** &RANNIQUILA &G***&w",
   "&W<<< &RDeJa En EsTaDo CriTiCo &W>>>&w"

};

char * p_generic_messages[24] =
{
	"falla", "roza", "roza", "aranya", "aranya", "corta", "corta",
	"golpea", "golpea", "raja", "raja", "hiere", "hiere", "demuele",
   "demuele", "DEVASTA", "&PMUTILA&w", "&PDESTROZA&w", "&rDESTRIPA&w",
   "&rDESFIGURA&w", "&y** &RDESCUARTIZA &y**&w", "&B** &REXTERMINA &B**&w", "&G*** &RANNIQUILA &G***&w",
   "&W<<< &RDeJa En EsTaDo CriTiCo &W>>>&w"

};

char ** const s_message_table[18] =
{
	s_generic_messages,	/* hit */
	s_blade_messages,	/* slice */
	s_blade_messages,	/* stab */
	s_blade_messages,	/* slash */
	s_blunt_messages,	/* whip */
	s_blade_messages,	/* claw */
	s_generic_messages,	/* blast */
	s_blunt_messages,	/* pound */
	s_blunt_messages,	/* crush */
	s_generic_messages,	/* grep */
	s_blade_messages,	/* bite */
	s_blade_messages,	/* pierce */
	s_blunt_messages,	/* suction */
	s_generic_messages,	/* bolt */
	s_generic_messages,	/* arrow */
	s_generic_messages,	/* dart */
	s_generic_messages,	/* stone */
	s_generic_messages	/* pea */
};

char ** const p_message_table[18] =
{
	p_generic_messages,	/* hit */
	p_blade_messages,	/* slice */
	p_blade_messages,	/* stab */
	p_blade_messages,	/* slash */
	p_blunt_messages,	/* whip */
	p_blade_messages,	/* claw */
	p_generic_messages,	/* blast */
	p_blunt_messages,	/* pound */
	p_blunt_messages,	/* crush */
	p_generic_messages,	/* grep */
	p_blade_messages,	/* bite */
	p_blade_messages,	/* pierce */
	p_blunt_messages,	/* suction */
	p_generic_messages,	/* bolt */
	p_generic_messages,	/* arrow */
	p_generic_messages,	/* dart */
	p_generic_messages,	/* stone */
	p_generic_messages	/* pea */
};

/* Weather constants - FB */
char * const temp_settings[MAX_CLIMATE] =
{
	"cold",
	"cool",
	"normal",
	"warm",
	"hot",
};

char * const precip_settings[MAX_CLIMATE] =
{
	"arid",
	"dry",
	"normal",
	"damp",
	"wet",
};

char * const wind_settings[MAX_CLIMATE] =
{
	"still",
	"calm",
	"normal",
	"breezy",
	"windy",
};

char * const preciptemp_msg[6][6] =
{
	/* precip = 0 */
	{
		"Frigid temperatures settle over the land",
		"It is bitterly cold",
		"The weather is crisp and dry",
		"A comfortable warmth sets in",
		"A dry heat warms the land",
		"Seething heat bakes the land"
	 },
	 /* precip = 1 */
	 {
	 	"A few flurries drift from the high clouds",
	 	"Frozen drops of rain fall from the sky",
	 	"An occasional raindrop falls to the ground",
	 	"Mild drops of rain seep from the clouds",
	 	"It is very warm, and the sky is overcast",
	 	"High humidity intensifies the seering heat"
	 },
	 /* precip = 2 */
	 {
	 	"A brief snow squall dusts the earth",
	 	"A light flurry dusts the ground",
	 	"Light snow drifts down from the heavens",
	 	"A light drizzle mars an otherwise perfect day",
	 	"A few drops of rain fall to the warm ground",
	 	"A light rain falls through the sweltering sky"
	 },
	 /* precip = 3 */
	 {
	 	"Snowfall covers the frigid earth",
	 	"Light snow falls to the ground",
	 	"A brief shower moistens the crisp air",
	 	"A pleasant rain falls from the heavens",
	 	"The warm air is heavy with rain",
	 	"A refreshing shower eases the oppresive heat"
	 },
	 /* precip = 4 */
	 {
	 	"Sleet falls in sheets through the frosty air",
	 	"Snow falls quickly, piling upon the cold earth",
	 	"Rain pelts the ground on this crisp day",
	 	"Rain drums the ground rythmically",
	 	"A warm rain drums the ground loudly",
	 	"Tropical rain showers pelt the seering ground"
	 },
	 /* precip = 5 */
	 {
	 	"A downpour of frozen rain covers the land in ice",
	 	"A blizzard blankets everything in pristine white",
	 	"Torrents of rain fall from a cool sky",
	 	"A drenching downpour obscures the temperate day",
	 	"Warm rain pours from the sky",
	 	"A torrent of rain soaks the heated earth"
	 }
};

char * const windtemp_msg[6][6] =
{
	/* wind = 0 */
	{
		"The frigid air is completely still",
		"A cold temperature hangs over the area",
		"The crisp air is eerily calm",
		"The warm air is still",
		"No wind makes the day uncomfortably warm",
		"The stagnant heat is sweltering"
	},
	/* wind = 1 */
	{
		"A light breeze makes the frigid air seem colder",
		"A stirring of the air intensifies the cold",
		"A touch of wind makes the day cool",
		"It is a temperate day, with a slight breeze",
		"It is very warm, the air stirs slightly",
		"A faint breeze stirs the feverish air"
	},
	/* wind = 2 */
	{
		"A breeze gives the frigid air bite",
		"A breeze swirls the cold air",
		"A lively breeze cools the area",
		"It is a temperate day, with a pleasant breeze",
		"Very warm breezes buffet the area",
		"A breeze ciculates the sweltering air"
	},
	/* wind = 3 */
	{
		"Stiff gusts add cold to the frigid air",
		"The cold air is agitated by gusts of wind",
		"Wind blows in from the north, cooling the area",
		"Gusty winds mix the temperate air",
		"Brief gusts of wind punctuate the warm day",
		"Wind attempts to cut the sweltering heat"
	},
	/* wind = 4 */
	{
		"The frigid air whirls in gusts of wind",
		"A strong, cold wind blows in from the north",
		"Strong wind makes the cool air nip",
		"It is a pleasant day, with gusty winds",
		"Warm, gusty winds move through the area",
		"Blustering winds punctuate the seering heat"
	},
	/* wind = 5 */
	{
		"A frigid gale sets bones shivering",
		"Howling gusts of wind cut the cold air",
		"An angry wind whips the air into a frenzy",
		"Fierce winds tear through the tepid air",
		"Gale-like winds whip up the warm air",
		"Monsoon winds tear the feverish air"
	}
};

char * const precip_msg[3] =
{
	"there is not a cloud in the sky",
	"pristine white clouds are in the sky",
	"thick, grey clouds mask the sun"
};

char * const wind_msg[6] =
{
	"there is not a breath of wind in the air",
	"a slight breeze stirs the air",
	"a breeze wafts through the area",
	"brief gusts of wind punctuate the air",
	"angry gusts of wind blow",
	"howling winds whip the air into a frenzy"
};


/*
 *  disciplinas Vampiricas y Sendas Taumaturgas  SiGo y SaNgUi  2001
 */

/*#define MAX_DISCIPLINAS          55*/
#define MAX_SENDAS                   20
#define DISCIPINA_RESERVADA              0
#define DISCIPLINA_ANIMALISMO     1
#define DISCIPLINA_AUSPEX         2
#define DISCIPLINA_BARDO          3
#define DISCIPLINA_CELERIDAD      4
#define DISCIPLINA_CONOCIMIENTO   5
#define DISCIPLINA_DAIMOINON      6
#define DISCIPLINA_DEMENTACION    7
#define DISCIPLINA_DOMINACION     8
#define DISCIPLINA_EXTINCION      9
#define DISCIPLINA_KINETISMO     10
#define DISCIPLINA_FORTALEZA     11
#define DISCIPLINA_MELOPOMENE    12
#define DISCIPLINA_MORTIS        13
#define DISCIPLINA_MYTHECERIA    14
#define DISCIPLINA_NIGROMANCIA   15
#define DISCIPLINA_NIHILISTICA   16
#define DISCIPLINA_OBEAH         17
#define DISCIPLINA_OBTENEBRACION 18
#define DISCIPLINA_OFUSCACION    19
#define DISCIPLINA_PRESENCIA     20
#define DISCIPLINA_POTENCIA      21
#define DISCIPLINA_PROTEAN       22
#define DISCIPLINA_QUIMERISMO    23
#define DISCIPLINA_SANGUINUS     24
#define DISCIPLINA_SERPENTIS     25
#define DISCIPLINA_SPIRITUS      26
#define DISCIPLINA_TANATOSIS     27
#define DISCIPLINA_TEMPORIS      28
#define DISCIPLINA_TAUMATURGIA   29
#define DISCIPLINA_DARKTARMA     30
#define DISCIPLINA_VISCISITUD    31
#define DISCIPLINA_VISCERATIKA   32

/* 31 Disciplinas Cainitas */

/*
 * Sendas taumaturgia
 */
#define SENDA_REGO_VITAE           0
#define SENDA_REGO_MOTUS           1
#define SENDA_CREO_IGNEM           2
#define SENDA_CONJURACION          3
#define SENDA_CORRUPCION           4
#define SENDA_NEPTUNO              5
#define SENDA_ESPIRITUAL           6
#define SENDA_ELEMENTAL            7
#define SENDA_ATMOSFERICA          8
#define SENDA_GEOMANCIA            9
#define SENDA_HIDEA_HIMAN         10
#define SENDA_MORFEO              11
/*
 * Sendas Nigromancia
 */
#define SENDA_SEPULCRO            12
#define SENDA_OSARIO              13
#define SENDA_CENIZAS             14
/*
 * Sendas Taumaturgia Oscura xD
 */
#define SENDA_PLACER              15    /* Cadenas de Placer */
#define SENDA_DESTRUCCION         16    /* Manos de Destruccion */
#define SENDA_INFIERNO            17    /* Fuegos del Infierno */
#define SENDA_PESTILENCIA         18
#define SENDA_FOBOS               19
#define SENDA_SECRETA             20    /* Senda del Conocimiento Secreto */
#define SENDA_TORTURA             21

/*
 * Musho pero ke musho kurro :(
 */



/*
 * The skill and spell table.
 * Slot numbers must never be changed as they appear in #OBJECTS sections.
 */
#define SLOT(n)	n
#define LI LEVEL_IMMORTAL

#undef AM
#undef AC
#undef AT
#undef AW
#undef AV
#undef AD
#undef AR
#undef AA

#undef LI
