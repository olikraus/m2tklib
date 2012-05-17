/*

  rapunzel.c 

  Short story Rapunzel, taken from gutenberg.org
  Copyright expired at least for US and Germany.
  See www.gutenberg.org for more information.

  m2tklib - Mini Interative Interface Toolkit Library
  u8glib - Universal 8bit Graphics Library
  
  Copyright (C) 2012  olikraus@gmail.com

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  
*/

#include "u8g.h"
#include "m2.h"
#include "m2ghu8g.h"

#if defined(__AVR__)
#include <avr/interrupt.h>
#include <avr/io.h>
#endif

// number of lines, must be a string.
#define LINES "4"



/*=========================================================================*/
/* menu definitions */

char part1[] M2_SECTION_PROGMEM =
"RAPUNZEL\n"
"\n"
"There were once a man\n"
"and a woman who had long\n"
"in vain wished for a\n"
"child. At length the\n"
"woman hoped that God was\n"
"about to grant her\n"
"desire. These people had\n"
"a little window at the\n"
"back of their house from\n"
"which a splendid garden\n"
"could be seen, which was\n"
"full of the most\n"
"beautiful flowers and\n"
"herbs. It was, however,\n"
"surrounded by a high\n"
"wall, and no one dared\n"
"to go into it because it\n"
"belonged to an\n"
"enchantress, who had\n"
"great power and was\n"
"dreaded by all the\n"
"world. One day the woman\n"
"was standing by this\n"
"window and looking down\n"
"into the garden, when\n"
"she saw a bed which was\n"
"planted with the most\n"
"beautiful rampion\n"
"(rapunzel), and it\n"
"looked so fresh and\n"
"green that she longed\n"
"for it, she quite pined\n"
"away, and began to look\n"
"pale and miserable. Then\n"
"her husband was alarmed,\n"
"and asked: 'What ails\n"
"you, dear wife?' 'Ah,'\n"
"she replied, 'if I can't\n"
"eat some of the rampion,\n"
"which is in the garden\n"
"behind our house, I\n"
"shall die.' The man, who\n"
"loved her, thought:\n"
"'Sooner than let your\n"
"wife die, bring her some\n"
"of the rampion yourself,\n"
"let it cost what it\n"
"will.' At twilight, he\n"
"clambered down over the\n"
"wall into the garden of\n"
"the enchantress, hastily\n"
"clutched a handful of\n"
"rampion, and took it to\n"
"his wife. She at once\n"
"made herself a salad of\n"
"it, and ate it greedily.\n"
"It tasted so good to\n"
"her--so very good, that\n"
"the next day she longed\n"
"for it three times as\n"
"much as before. If he\n"
"was to have any rest,\n"
"her husband must once\n"
"more descend into the\n"
"garden. In the gloom of\n"
"evening therefore, he\n"
"let himself down again;\n"
"but when he had\n"
"clambered down the wall\n"
"he was terribly afraid,\n"
"for he saw the\n"
"enchantress standing\n"
"before him. 'How can you\n"
"dare,' said she with\n"
"angry look, 'descend\n"
"into my garden and steal\n"
"my rampion like a thief?\n"
"You shall suffer for\n"
"it!' 'Ah,' answered he,\n"
"'let mercy take the\n"
"place of justice, I only\n"
"made up my mind to do it\n"
"out of necessity. My\n"
"wife saw your rampion\n"
"from the window, and\n"
"felt such a longing for\n"
"it that she would have\n"
"died if she had not got\n"
"some to eat.' Then the\n"
"enchantress allowed her\n"
"anger to be softened,\n"
"and said to him: 'If the\n"
"case be as you say, I\n"
"will allow you to take\n"
"away with you as much\n"
"rampion as you will,\n"
"only I make one\n"
"condition, you must give\n"
"me the child which your\n"
"wife will bring into the\n"
"world; it shall be well\n"
"treated, and I will care\n"
"for it like a mother.'\n"
"The man in his terror\n"
"consented to everything,\n"
"and when the woman was\n"
"brought to bed, the\n"
"enchantress appeared at\n"
"once, gave the child the\n"
"name of Rapunzel, and\n"
"took it away with her.\n"
"\n"
"Rapunzel grew into the\n"
"most beautiful child\n"
"under the sun. When she\n"
"was twelve years old,\n"
"the enchantress shut her\n"
"into a tower, which lay\n"
"in a forest, and had\n"
"neither stairs nor door,\n"
"but quite at the top was\n"
"a little window. When\n"
"the enchantress wanted\n"
"to go in, she placed\n"
"herself beneath it and\n"
"cried:\n"
"\n"
" 'Rapunzel, Rapunzel,\n"
"  Let down your hair to\n"
"  me.'\n"
"\n"
"Rapunzel had magnificent\n"
"long hair, fine as spun\n"
"gold, and when she heard\n"
"the voice of the\n"
"enchantress she\n"
"unfastened her braided\n"
"tresses, wound them\n"
"round one of the hooks\n"
"of the window above, and\n"
"then the hair fell\n"
"twenty ells down, and\n"
"the enchantress climbed\n"
"up by it.\n"
"\n"
"After a year or two, it\n"
"came to pass that the\n"
"king's son rode through\n"
"the forest and passed by\n"
"the tower. Then he heard\n"
"a song, which was so\n"
"charming that he stood\n"
"still and listened. This\n"
"was Rapunzel, who in her\n"
"solitude passed her time\n"
"in letting her sweet\n"
"voice resound. The\n"
"king's son wanted to\n"
"climb up to her, and\n"
"looked for the door of\n"
"the tower, but none was\n"
"to be found. He rode\n"
"home, but the singing\n"
"had so deeply touched\n"
"his heart, that every\n"
"day he went out into the\n"
"forest and listened to\n"
"it. Once when he was\n"
"thus standing behind a\n"
"tree, he saw that an\n"
"enchantress came there,\n"
"and he heard how she\n"
"cried:\n"
"\n"
" 'Rapunzel, Rapunzel,\n"
"  Let down your hair to\n"
"  me.'\n"
;

char part2[] M2_SECTION_PROGMEM =
"Then Rapunzel let down\n"
"the braids of her hair,\n"
"and the enchantress\n"
"climbed up to her. 'If\n"
"that is the ladder by\n"
"which one mounts, I too\n"
"will try my fortune,'\n"
"said he, and the next\n"
"day when it began to\n"
"grow dark, he went to\n"
"the tower and cried:\n"
"\n"
" 'Rapunzel, Rapunzel,\n"
"  Let down your hair to\n"
"  me.'\n"
"\n"
"Immediately the hair\n"
"fell down and the king's\n"
"son climbed up.\n"
"\n"
"At first Rapunzel was\n"
"terribly frightened when\n"
"a man, such as her eyes\n"
"had never yet beheld,\n"
"came to her; but the\n"
"king's son began to talk\n"
"to her quite like a\n"
"friend, and told her\n"
"that his heart had been\n"
"so stirred that it had\n"
"let him have no rest,\n"
"and he had been forced\n"
"to see her. Then\n"
"Rapunzel lost her fear,\n"
"and when he asked her if\n"
"she would take him for\n"
"her husband, and she saw\n"
"that he was young and\n"
"handsome, she thought:\n"
"'He will love me more\n"
"than old Dame Gothel\n"
"does'; and she said yes,\n"
"and laid her hand in\n"
"his. She said: 'I will\n"
"willingly go away with\n"
"you, but I do not know\n"
"how to get down. Bring\n"
"with you a skein of silk\n"
"every time that you\n"
"come, and I will weave a\n"
"ladder with it, and when\n"
"that is ready I will\n"
"descend, and you will\n"
"take me on your horse.'\n"
"They agreed that until\n"
"that time he should come\n"
"to her every evening,\n"
"for the old woman came\n"
"by day. The enchantress\n"
"remarked nothing of\n"
"this, until once\n"
"Rapunzel said to her:\n"
"'Tell me, Dame Gothel,\n"
"how it happens that you\n"
"are so much heavier for\n"
"me to draw up than the\n"
"young king's son--he is\n"
"with me in a moment.'\n"
"'Ah! you wicked child,'\n"
"cried the enchantress.\n"
"'What do I hear you say!\n"
"I thought I had\n"
"separated you from all\n"
"the world, and yet you\n"
"have deceived me!' In\n"
"her anger she clutched\n"
"Rapunzel's beautiful\n"
"tresses, wrapped them\n"
"twice round her left\n"
"hand, seized a pair of\n"
"scissors with the right,\n"
"and snip, snap, they\n"
"were cut off, and the\n"
"lovely braids lay on the\n"
"ground. And she was so\n"
"pitiless that she took\n"
"poor Rapunzel into a\n"
"desert where she had to\n"
"live in great grief and\n"
"misery.\n"
"\n"
"On the same day that she\n"
"cast out Rapunzel,\n"
"however, the enchantress\n"
"fastened the braids of\n"
"hair, which she had cut\n"
"off, to the hook of the\n"
"window, and when the\n"
"king's son came and\n"
"cried:\n"
"\n"
" 'Rapunzel, Rapunzel,\n"
"  Let down your hair to\n"
"  me.'\n"
"\n"
"she let the hair down.\n"
"The king's son ascended,\n"
"but instead of finding\n"
"his dearest Rapunzel, he\n"
"found the enchantress,\n"
"who gazed at him with\n"
"wicked and venomous\n"
"looks. 'Aha!' she cried\n"
"mockingly, 'you would\n"
"fetch your dearest, but\n"
"the beautiful bird sits\n"
"no longer singing in the\n"
"nest; the cat has got\n"
"it, and will scratch out\n"
"your eyes as well.\n"
"Rapunzel is lost to you;\n"
"you will never see her\n"
"again.' The king's son\n"
"was beside himself with\n"
"pain, and in his despair\n"
"he leapt down from the\n"
"tower. He escaped with\n"
"his life, but the thorns\n"
"into which he fell\n"
"pierced his eyes. Then\n"
"he wandered quite blind\n"
"about the forest, ate\n"
"nothing but roots and\n"
"berries, and did naught\n"
"but lament and weep over\n"
"the loss of his dearest\n"
"wife. Thus he roamed\n"
"about in misery for some\n"
"years, and at length\n"
"came to the desert where\n"
"Rapunzel, with the twins\n"
"to which she had given\n"
"birth, a boy and a girl,\n"
"lived in wretchedness.\n"
"He heard a voice, and it\n"
"seemed so familiar to\n"
"him that he went towards\n"
"it, and when he\n"
"approached, Rapunzel\n"
"knew him and fell on his\n"
"neck and wept. Two of\n"
"her tears wetted his\n"
"eyes and they grew clear\n"
"again, and he could see\n"
"with them as before. He\n"
"led her to his kingdom\n"
"where he was joyfully\n"
"received, and they lived\n"
"for a long time\n"
"afterwards, happy and\n"
"contented.\n"
;


M2_EXTERN_ALIGN(el_top);


uint8_t total_lines1 = 0;
uint8_t first_visible_line1 = 0;

uint8_t total_lines2 = 0;
uint8_t first_visible_line2 = 0;

void goto_top_fn(m2_el_fnarg_p fnarg) {
  m2_SetRoot(&el_top);
}

M2_INFOP(el_info1, "W60l" LINES, &first_visible_line1, &total_lines1, part1, goto_top_fn);
M2_VSB(el_vsb1, "W2r1l" LINES, &first_visible_line1, &total_lines1);
M2_LIST(el_list1) = { &el_info1, &el_vsb1};
M2_HLIST(el_part1, NULL, el_list1);

M2_INFOP(el_info2, "W60l" LINES, &first_visible_line2, &total_lines2, part2, goto_top_fn);
M2_VSB(el_vsb2, "W2r1l" LINES, &first_visible_line2, &total_lines2);
M2_LIST(el_list2) = { &el_info2, &el_vsb2};
M2_HLIST(el_part2, NULL, el_list2);

M2_LABEL(el_goto_title, NULL, "Rapunzel");
M2_ROOT(el_goto_part1, NULL, "Part 1", &el_part1);
M2_ROOT(el_goto_part2, NULL, "Part 2", &el_part2);
M2_LIST(list_menu) = {&el_goto_title, &el_goto_part1, &el_goto_part2};
M2_VLIST(el_menu_vlist, NULL, list_menu);
M2_ALIGN(el_top, "W64H64", &el_menu_vlist);

/*=========================================================================*/
/* global variables and objects */

u8g_t u8g;

/*=========================================================================*/
/* controller, u8g and m2 setup */

void setup(void)
{  
  /*
    Test Envionment, ATMEGA with the following settings:
    CS: PORTB, Bit 2
    A0: PORTB, Bit 1
    SCK: PORTB, Bit 5
    MOSI: PORTB, Bit 3
  */
  /* 1. Setup and create u8g device */
  /* u8g_InitSPI(&u8g, &u8g_dev_st7565_dogm132_sw_spi, PN(1, 5), PN(1, 3), PN(1, 2), PN(1, 1), U8G_PIN_NONE); */
  u8g_InitHWSPI(&u8g, &u8g_dev_st7565_dogm132_hw_spi, PN(1, 2), PN(1, 1), U8G_PIN_NONE);

  /* 2. Setup m2 */
  m2_Init(&el_top, m2_es_avr_u8g, m2_eh_4bs, m2_gh_u8g_bfs);

  /* 3. Connect u8g display to m2  */
  m2_SetU8g(&u8g, m2_u8g_box_icon);

  /* 4. Set a font, use normal u8g_font's */
  m2_SetFont(0, (const void *)u8g_font_5x8r);
	
  /* 5. Define keys */
  m2_SetPin(M2_KEY_EXIT, PN(3, 5));
  m2_SetPin(M2_KEY_SELECT, PN(3, 6));
  m2_SetPin(M2_KEY_NEXT, PN(3, 7));
  m2_SetPin(M2_KEY_PREV, PN(1, 7));
}

/*=========================================================================*/
/* system setup */

void sys_init(void)
{
#if defined(__AVR__)
  /* select minimal prescaler (max system speed) */
  CLKPR = 0x80;
  CLKPR = 0x00;
#endif
}

/*=========================================================================*/
/* u8g draw procedure (body of picture loop) */

/* draw procedure of the u8g picture loop */
void draw(void)
{	
  /* call the m2 draw procedure */
  m2_Draw();
}

/*=========================================================================*/
/* main procedure with u8g picture loop */

int main(void)
{
  /* setup controller */
  sys_init();
	
  /* setup u8g and m2 libraries */
  setup();

  /* application main loop */
  for(;;)
  {  
    m2_CheckKey();
    if ( m2_HandleKey() ) 
    {
      /* picture loop */
      u8g_FirstPage(&u8g);
      do
      {
	draw();
        m2_CheckKey();
      } while( u8g_NextPage(&u8g) );
    }
  }  
}

