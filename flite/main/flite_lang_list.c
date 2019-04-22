/* Generated automatically from make_lang_list */

#include "flite.h"

void utf8_grapheme_lang_init(cst_voice *v);
cst_lexicon *utf8_grapheme_lex_init(void);

void usenglish_init(cst_voice *v);
cst_lexicon *cmulex_init(void);

void cmu_indic_lang_init(cst_voice *v);
cst_lexicon *cmu_indic_lex_init(void);


void flite_set_lang_list(void)
{
   flite_add_lang("utf8_grapheme",utf8_grapheme_lang_init,utf8_grapheme_lex_init);
   flite_add_lang("eng",usenglish_init,cmulex_init);
   flite_add_lang("usenglish",usenglish_init,cmulex_init);
   flite_add_lang("cmu_indic_lang",cmu_indic_lang_init,cmu_indic_lex_init);
}

