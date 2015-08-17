/**********************************************************************
Each of the companies; Qualcomm, and Lucent, and Motorola (hereinafter 
referred to individually as "Source" or collectively as "Sources") do 
hereby state:

To the extent to which the Source(s) may legally and freely do so, the 
Source(s), upon submission of a Contribution, grant(s) a free, 
irrevocable, non-exclusive, license to the Third Generation Partnership 
Project 2 (3GPP2) and its Organizational Partners: ARIB, CCSA, TIA, TTA, 
and TTC, under the Source's copyright or copyright license rights in the 
Contribution, to, in whole or in part, copy, make derivative works, 
perform, display and distribute the Contribution and derivative works 
thereof consistent with 3GPP2's and each Organizational Partner's 
policies and procedures, with the right to (i) sublicense the foregoing 
rights consistent with 3GPP2's and each Organizational Partner's  policies 
and procedures and (ii) copyright and sell, if applicable) in 3GPP2's name 
or each Organizational Partner's name any 3GPP2 or transposed Publication 
even though this Publication may contain the Contribution or a derivative 
work thereof.  The Contribution shall disclose any known limitations on 
the Source's rights to license as herein provided.

When a Contribution is submitted by the Source(s) to assist the 
formulating groups of 3GPP2 or any of its Organizational Partners, it 
is proposed to the Committee as a basis for discussion and is not to 
be construed as a binding proposal on the Source(s).  The Source(s) 
specifically reserve(s) the right to amend or modify the material 
contained in the Contribution. Nothing contained in the Contribution 
shall, except as herein expressly provided, be construed as conferring 
by implication, estoppel or otherwise, any license or right under (i) 
any existing or later issuing patent, whether or not the use of 
information in the document necessarily employs an invention of any 
existing or later issued patent, (ii) any copyright, (iii) any 
trademark, or (iv) any other intellectual property right.

With respect to the Software necessary for the practice of any or 
all Normative portions of the EVRC-B Variable Rate Speech Codec as 
it exists on the date of submittal of this form, should the EVRC-B be 
approved as a Specification or Report by 3GPP2, or as a transposed 
Standard by any of the 3GPP2's Organizational Partners, the Source(s) 
state(s) that a worldwide license to reproduce, use and distribute the 
Software, the license rights to which are held by the Source(s), will 
be made available to applicants under terms and conditions that are 
reasonable and non-discriminatory, which may include monetary compensation, 
and only to the extent necessary for the practice of any or all of the 
Normative portions of the EVRC-B or the field of use of practice of the 
EVRC-B Specification, Report, or Standard.  The statement contained above 
is irrevocable and shall be binding upon the Source(s).  In the event 
the rights of the Source(s) in and to copyright or copyright license 
rights subject to such commitment are assigned or transferred, the 
Source(s) shall notify the assignee or transferee of the existence of 
such commitments.
*******************************************************************/
/*===================================================================*/
/* Conexant Systems Inc., Ericsson Inc., Lucent Technologies Inc.,   */
/* Motorola Inc., Nokia Inc, Nortel Networks Corporation, and        */
/* Qualcomm Inc. do hereby grant a free, irrevocable license to      */
/* 3GPP2 and its Organizational Partners (OPs) to incorporate text   */
/* or other copyrightable material contained in the above-referenced */
/* document and any modifications thereof in the creation of 3GPP2   */
/* publications; to copyright and sell in Organizational Partner's   */
/* name, any Organizational Partner's standards publication even     */
/* though it may include portions of the text or other copyrightable */
/* material contained in such document; and at the Organizational    */
/* Partner's sole discretion to permit others to reproduce in whole  */
/* or in part such text or other copyrightable material or the       */
/* resulting Organizational Partner's standards publication.         */
/* Conexant Systems Inc., Ericsson Inc, Lucent Technologies Inc.,    */
/* Motorola Inc., Nokia Inc, Nortel Networks Corporation, and        */
/* Qualcomm Inc. do hereby agree that it will grant licenses under   */
/* any copyrights of such undersigned party to third parties on      */
/* reasonable, non-discriminatory terms and conditions.              */
/*                                                                   */
/*-------------------------------------------------------------------*/
/* Nothing contained herein shall be construed as conferring by      */
/* implication, estoppel or otherwise, any license or right under    */
/* any existing or later issuing patent, whether or not the use of   */ 
/* information in the document necessarily employs an invention of   */
/* any existing or later issued patent, or copyright, but Conexant   */
/* Systems Inc., Ericsson Inc., Lucent Technologies Inc., Motorola   */
/* Inc., Nokia Inc, Nortel Networks Corporation, and Qualcomm Inc.   */
/* will make a copyright license available as hereinabove previously */
/* stated.                                                           */
/*                                                                   */
/*-------------------------------------------------------------------*/
/*===================================================================*/

/*********************************************************************
*   get_tty_state()
*
**********************************************************************/
#include <stdio.h>
#include "tty.h"
#include "basic_op.h"


#define ONSET_BITS          2

void get_tty_state(
    Word16   *ttyState,              /* (i/o): TTY State                   */
    Word16   first_tty_char_flag,    /* (i): first char detected flag      */
    Word16   tty_bit_hist[],         /* (i): TTY bit history buffer        */
    Word16   tty_bit_len_hist[]      /* (i): TTY bit length history buffer */
)
{
    Word16   j;
    Word16   index;
    Word16   temp;



                                                                
    if( sub(tty_bit_len_hist[TTY_BIT_HIST_LEN-1],MIN_BIT_LEN) > 0 )
    {
        index = TTY_BIT_HIST_LEN-1;                             
    }
    else
    {
        index = STOP_BIT_IDX;                                   
    }

                                                                
    if( sub(tty_bit_hist[index],LOGIC_1) == 0
        && sub(tty_bit_hist[index-1],LOGIC_0) == 0 )
    {
        *ttyState = TTY_ONSET;                              
    }

    temp = index;                                           
    for( j=temp ; j >= START_BIT_IDX  ; j-- )
    {
                                                            
        if( sub(tty_bit_hist[j],LOGIC_0) == 0 )
        {
            temp = j;
        }
        else
        {
                                                            
            if( sub(tty_bit_hist[j],LOGIC_1) != 0 )
            {
                break;
            }
        }
    }

    /* Zero the bad dit counter */

    if( sub(temp,MEM_BIT_IDX) == 0 || sub(tty_bit_hist[temp],LOGIC_0) != 0 )
    {
        tty_bit_len_hist[MEM_BIT_IDX] = 0;
    }

    if( sub(tty_bit_hist[temp],LOGIC_0) != 0 )
    {
        for( j=0 ; j < TTY_BIT_HIST_LEN ; j++ )
        {
            tty_bit_hist[j] = UNKNOWN;
        }
    }

                                                            
    if( sub(temp,sub(index,ONSET_BITS-1)) > 0 )
    {
        *ttyState = NON_TTY_MODE;                       


        return;
    }
    else
    {
                                                           
        if( sub(temp,START_BIT_IDX) <= 0  )
        {
            *ttyState = NON_TTY_MODE;                       


            return;
        }
                                                            
        if( sub(first_tty_char_flag,1) == 0 )
        {
            *ttyState = TTY_ONSET;                              
        }
    }

                                                            
    if( sub(tty_bit_hist[TTY_BIT_HIST_LEN-1],UNKNOWN) == 0 )
    {
        *ttyState = NON_TTY_MODE;                           


        return;
    }



    return;
}
