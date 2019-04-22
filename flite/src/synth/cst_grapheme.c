/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                         Copyright (c) 2012                            */
/*                        All Rights Reserved.                           */
/*                                                                       */
/*  Permission is hereby granted, free of charge, to use and distribute  */
/*  this software and its documentation without restriction, including   */
/*  without limitation the rights to use, copy, modify, merge, publish,  */
/*  distribute, sublicense, and/or sell copies of this work, and to      */
/*  permit persons to whom this work is furnished to do so, subject to   */
/*  the following conditions:                                            */
/*   1. The code must retain the above copyright notice, this list of    */
/*      conditions and the following disclaimer.                         */
/*   2. Any modifications must be clearly marked as such.                */
/*   3. Original authors' names are not deleted.                         */
/*   4. The authors' names are not used to endorse or promote products   */
/*      derived from this software without specific prior written        */
/*      permission.                                                      */
/*                                                                       */
/*  CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK         */
/*  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      */
/*  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   */
/*  SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE      */
/*  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    */
/*  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   */
/*  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          */
/*  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       */
/*  THIS SOFTWARE.                                                       */
/*                                                                       */
/*************************************************************************/
/*             Author:  Alan W Black (awb@cs.cmu.edu)                    */
/*               Date:  May 2012                                         */
/*************************************************************************/
/*                                                                       */
/*  Generic Language and Lexicon modules for utf8 Grapheme based voices  */
/*                                                                       */
/*************************************************************************/
#include "flite.h"
#include "cst_val.h"
#include "cst_voice.h"
#include "cst_lexicon.h"
#include "cst_ffeatures.h"

void utf8_grapheme_lang_init(cst_voice *v)
{
    /* Set generic language stuff */

    /* utterance break function */
    feat_set(v->features,"utt_break",breakfunc_val(&default_utt_break));

    /* Phoneset -- need to get this from voice */

    /* Get information from voice and add to lexicon */

    /* Text analyser -- whitespace defaults */
    feat_set_string(v->features,"text_whitespace",
                    cst_ts_default_whitespacesymbols);
    feat_set_string(v->features,"text_postpunctuation",
                    cst_ts_default_prepunctuationsymbols);
    feat_set_string(v->features,"text_prepunctuation",
                    cst_ts_default_postpunctuationsymbols);
    feat_set_string(v->features,"text_singlecharsymbols",
                    cst_ts_default_singlecharsymbols);

    /* We probably can do something here -- but not yet */
      /* Pos tagger (gpos)/induced pos */
    /* Phrasing */
    /* Intonation, Duration and F0 -- part of cg */

    /* Default ffunctions */
    basic_ff_register(v->ffunctions);

    return;
}

cst_lexicon *utf8_grapheme_lex_init(void)
{
    /* Should it be global const or dynamic */
    /* Can make lts_rules just a cart tree like others */
    cst_lexicon *l;

    l = cst_alloc(cst_lexicon,1);
    l->name = "utf8_grapheme";

    return l;

}
