#ifndef AES128_SHARING_H
#define AES128_SHARING_H

#include <stdint.h>

/**********************************************************
 * this file contains the full implementation of the
 * AES-128 procedure in an n-share version. So basically,
 * each + (resp. *) operation is replaced by a call to 
 * add_gadget_function (resp. mult_gadget_function), and 
 * whenever a variable needs to be copied, a call to
 * copy_gadget_function is used with the necessary number
 * of calls. In addition, all variables from the standard
 * AES-128 implementation, are now replaced with n-share
 * variables of the same type (uint8_t)
**********************************************************/

void exp254_sharing(uint8_t *x, uint8_t * out);

void get_sbox_value_sharing(uint8_t * x, uint8_t * out);

void get_inv_sbox_value_sharing(uint8_t * x, uint8_t * out);


/**********************************************************
 * For shift_rows and inv_shift_rows, we are shifting 
 * complete arrays instead of single scalars (we now have
 * n-share variables). So to avoid looping over all shares
 * and copying whole arrays, we use dynamic indexing with
 * the variable ind_state and lightly tweak the code of
 * the AES encryption and decryption functions to use
 * ind_state.
**********************************************************/
void shift_rows_sharing(uint8_t ** state, uint8_t * ind_state);

void inv_shift_rows_sharing(uint8_t ** state, uint8_t * ind_state);

void mix_columns_sharing(uint8_t ** state, uint8_t ** ciphertext, uint8_t * ind_state);

void inv_mix_columns_sharing(uint8_t ** state, uint8_t ** plaintext, uint8_t * ind_state);

void aes_encrypt_128_sharing(uint8_t **roundkeys, uint8_t **plaintext, uint8_t **ciphertext);

void aes_decrypt_128_sharing(uint8_t **roundkeys, uint8_t **ciphertext, uint8_t **plaintext);

#endif
