#include "aes128.h"
#include "gf256.h"

/**********************************************************
 * this file contains the full implementation of the
 * AES-128 standard procedure
**********************************************************/


uint8_t RC[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36};

void aes_key_schedule_128(uint8_t *key, uint8_t *roundkeys) {

    uint8_t temp[4];
    uint8_t *last4bytes; // point to the last 4 bytes of one round
    uint8_t *lastround;
    uint8_t i;

    for (i = 0; i < 16; ++i) {
        *roundkeys++ = *key++;
    }

    last4bytes = roundkeys-4;
    for (i = 0; i < AES_ROUNDS; ++i) {
        // k0-k3 for next round
        temp[3] = get_sbox_value(*last4bytes++);
        temp[0] = get_sbox_value(*last4bytes++);
        temp[1] = get_sbox_value(*last4bytes++);
        temp[2] = get_sbox_value(*last4bytes++);
        temp[0] ^= RC[i];
        lastround = roundkeys-16;
        *roundkeys++ = temp[0] ^ *lastround++;
        *roundkeys++ = temp[1] ^ *lastround++;
        *roundkeys++ = temp[2] ^ *lastround++;
        *roundkeys++ = temp[3] ^ *lastround++;
        // k4-k7 for next round        
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        // k8-k11 for next round
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        // k12-k15 for next round
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
        *roundkeys++ = *last4bytes++ ^ *lastround++;
    }
}


uint8_t RC192[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6C, 0xD8};

void aes_key_schedule_192(uint8_t *key, uint8_t *roundkeys) {
    uint8_t temp[4];
    uint8_t *last4bytes;
    uint8_t *prev_round;
    int i;

    // 1. Copy 24-byte initial key
    for (i = 0; i < 24; ++i) {
        *roundkeys++ = *key++;
    }

    last4bytes = roundkeys - 4; // Points to the last 4 bytes of the initial key
    
    // 2. Expand remaining 46 words (52-6=46)
    for (i = 0; i < 8; ++i) {  // AES-192 requires 8 rounds of key expansion
        // Core transformation processing
        if (1) {  // The first word of each round requires special processing
            // Save current last 4-byte value
            uint8_t b0 = last4bytes[0];
            uint8_t b1 = last4bytes[1];
            uint8_t b2 = last4bytes[2];
            uint8_t b3 = last4bytes[3];
            
            // RotWord: Circular left shift [a,b,c,d] -> [b,c,d,a]
            uint8_t rot[4] = {b1, b2, b3, b0};
            
            // SubWord: Apply S-box
            temp[0] = get_sbox_value(rot[0]);
            temp[1] = get_sbox_value(rot[1]);
            temp[2] = get_sbox_value(rot[2]);
            temp[3] = get_sbox_value(rot[3]);
            
            // XOR with round constant
            temp[0] ^= RC192[i];
        } else {
            // Directly use last 4 bytes
            temp[0] = last4bytes[0];
            temp[1] = last4bytes[1];
            temp[2] = last4bytes[2];
            temp[3] = last4bytes[3];
        }
        
        // 3. Calculate new word: w[i] = w[i-6] ^ temp
        prev_round = roundkeys - 24; // Points to w[i-6]
        *roundkeys++ = prev_round[0] ^ temp[0];
        *roundkeys++ = prev_round[1] ^ temp[1];
        *roundkeys++ = prev_round[2] ^ temp[2];
        *roundkeys++ = prev_round[3] ^ temp[3];
        
        // 4. Update last4bytes to point to newly generated 4 bytes
        last4bytes = roundkeys - 4;
        
        // 5. Generate remaining 5 words for current round
        for (int j = 0; j < 5; j++) {
            *roundkeys++ = last4bytes[0] ^ prev_round[4+j*4];
            *roundkeys++ = last4bytes[1] ^ prev_round[5+j*4];
            *roundkeys++ = last4bytes[2] ^ prev_round[6+j*4];
            *roundkeys++ = last4bytes[3] ^ prev_round[7+j*4];
            last4bytes = roundkeys - 4;
        }
    }
}





// Expand round constant array (14 values for AES-256's 14 rounds)
uint8_t RC256[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 
                0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d};

void aes_key_schedule_256(uint8_t *key, uint8_t *roundkeys) {
    uint8_t temp[4];
    uint8_t* last4bytes;  // Points to last 4 bytes of previous round
    uint8_t* lastround;   // Points to start of previous round
    uint8_t i, j;
    uint8_t *original_keys = roundkeys;  // Save initial position

    // 1. Copy initial 256-bit key (32 bytes)
    for (i = 0; i < 32; ++i) {
        *roundkeys++ = *key++;
    }

    // 2. Key expansion (generate 14 round keys, 7 groups total)
    for (i = 0; i < 7; ++i) {
        // ====== First half: Core nonlinear transformation ======
        last4bytes = roundkeys - 4;  // Last 4 bytes of the previous round
        // RotWord + SubBytes
        temp[0] = get_sbox_value(last4bytes[1]);  // Circular left shift：1->0
        temp[1] = get_sbox_value(last4bytes[2]);  // 2->1
        temp[2] = get_sbox_value(last4bytes[3]);  // 3->2
        temp[3] = get_sbox_value(last4bytes[0]);  // 0->3
        // Rcon XOR (using even indices: 0,2,4...)
        temp[0] ^= RC256[2*i];
        
        // XOR with first word of previous round
        lastround = roundkeys - 32;  // Start position of the previous round
        *roundkeys++ = temp[0] ^ *lastround++;
        *roundkeys++ = temp[1] ^ *lastround++;
        *roundkeys++ = temp[2] ^ *lastround++;
        *roundkeys++ = temp[3] ^ *lastround++;
        
        // Generate next 3 words (linear XOR)
        for (j = 0; j < 3; j++) {
            *roundkeys++ = *last4bytes++ ^ *lastround++;
            *roundkeys++ = *last4bytes++ ^ *lastround++;
            *roundkeys++ = *last4bytes++ ^ *lastround++;
            *roundkeys++ = *last4bytes++ ^ *lastround++;
        }
        
        // ====== Second half: AES-256 special processing ======
        last4bytes = roundkeys - 4;  // Update the position of the last 4 bytes
        // SubBytes（AES-256 specific）
        temp[0] = get_sbox_value(last4bytes[0]);
        temp[1] = get_sbox_value(last4bytes[1]);
        temp[2] = get_sbox_value(last4bytes[2]);
        temp[3] = get_sbox_value(last4bytes[3]);
        
        // XOR with corresponding word from previous round
        lastround = roundkeys - 32;  // eposition
        *roundkeys++ = temp[0] ^ *lastround++;
        *roundkeys++ = temp[1] ^ *lastround++;
        *roundkeys++ = temp[2] ^ *lastround++;
        *roundkeys++ = temp[3] ^ *lastround++;
        
        // Generate remaining 3 words (linear XOR)
        for (j = 0; j < 3; j++) {
            *roundkeys++ = *last4bytes++ ^ *lastround++;
            *roundkeys++ = *last4bytes++ ^ *lastround++;
            *roundkeys++ = *last4bytes++ ^ *lastround++;
            *roundkeys++ = *last4bytes++ ^ *lastround++;
        }
    }
}





uint8_t exp254(uint8_t x){
	uint8_t tmp = Multiply(x, x);
	
	tmp = Multiply(tmp, tmp);
	
	tmp = Multiply(tmp, tmp);
	
	tmp = Multiply(tmp, x);
	
	tmp = Multiply(tmp, tmp);
	
	uint8_t tmp2 = Multiply(tmp, x);
	tmp = Multiply(tmp, tmp);
	
	tmp2 = Multiply(tmp2, tmp);
	tmp = Multiply(tmp, tmp);
	
	tmp = Multiply(tmp, tmp2);
	
	return Multiply(tmp, tmp);
}

uint8_t get_sbox_value(uint8_t x){
	//Exponentiation
	x = exp254(x);
	
	//Affine function
	uint8_t res = Multiply(207, x);
	res = Multiply(res, res);
	
	uint8_t tmp = Multiply(22, x);
	res = Add(res, tmp);
	res = Multiply(res, res);
	
	tmp = Multiply(1, x);
	res = Add(res, tmp);
	res = Multiply(res, res);
	
	tmp = Multiply(73, x);
	res = Add(res, tmp);
	res = Multiply(res, res);
	
	tmp = Multiply(204, x);
	res = Add(res, tmp);
	res = Multiply(res, res);
	
	tmp = Multiply(168, x);
	res = Add(res, tmp);
	res = Multiply(res, res);
	
	tmp = Multiply(238, x);
	res = Add(res, tmp);
	res = Multiply(res, res);
	
	tmp = Multiply(5, x);
	res = Add(res, tmp);
	
	res = Add(99, res);
	
	return res;	
}


uint8_t get_inv_sbox_value(uint8_t x){
	//Affine function
	uint8_t res = Multiply(147, x);
	res = Multiply(res, res);
	
	uint8_t tmp = Multiply(146, x);
	res = Add(res, tmp);
	res = Multiply(res, res);
	
	tmp = Multiply(190, x);
	res = Add(res, tmp);
	res = Multiply(res, res);
	
	tmp = Multiply(41, x);
	res = Add(res, tmp);
	res = Multiply(res, res);
	
	tmp = Multiply(73, x);
	res = Add(res, tmp);
	res = Multiply(res, res);
	
	tmp = Multiply(139, x);
	res = Add(res, tmp);
	res = Multiply(res, res);
	
	tmp = Multiply(79, x);
	res = Add(res, tmp);
	res = Multiply(res, res);
	
	tmp = Multiply(5, x);
	res = Add(res, tmp);
	
	x = Add(5, res);
	
	//Exponentiation
	return exp254(x);
	
}


void shift_rows(uint8_t *state) {
    uint8_t temp;
    // row1
    temp        = *(state+1);
    *(state+1)  = *(state+5);
    *(state+5)  = *(state+9);
    *(state+9)  = *(state+13);
    *(state+13) = temp;
    // row2
    temp        = *(state+2);
    *(state+2)  = *(state+10);
    *(state+10) = temp;
    temp        = *(state+6);
    *(state+6)  = *(state+14);
    *(state+14) = temp;
    // row3
    temp        = *(state+15);
    *(state+15) = *(state+11);
    *(state+11) = *(state+7);
    *(state+7)  = *(state+3);
    *(state+3)  = temp;
}


void inv_shift_rows(uint8_t *state) {
    uint8_t temp;
    // row1
    temp        = *(state+13);
    *(state+13) = *(state+9);
    *(state+9)  = *(state+5);
    *(state+5)  = *(state+1);
    *(state+1)  = temp;
    // row2
    temp        = *(state+14);
    *(state+14) = *(state+6);
    *(state+6)  = temp;
    temp        = *(state+10);
    *(state+10) = *(state+2);
    *(state+2)  = temp;
    // row3
    temp        = *(state+3);
    *(state+3)  = *(state+7);
    *(state+7)  = *(state+11);
    *(state+11) = *(state+15);
    *(state+15) = temp;
}


void aes_encrypt_128(uint8_t *roundkeys, uint8_t *plaintext, uint8_t *ciphertext) {

    uint8_t state[AES_BLOCK_SIZE], t;
    uint8_t i, j;

    // first AddRoundKey
    for ( i = 0; i < AES_BLOCK_SIZE; ++i ) {
        ciphertext[i] = Add(plaintext[i], *roundkeys++);
    }

    // 9 rounds
    for (j = 1; j < AES_ROUNDS; ++j) {

        // SubBytes
        for (i = 0; i < AES_BLOCK_SIZE; ++i) {
            state[i] = get_sbox_value(ciphertext[i]);
        }
        shift_rows(state);
        /*
         * MixColumns 
         * [02 03 01 01]   [s0  s4  s8  s12]
         * [01 02 03 01] . [s1  s5  s9  s13]
         * [01 01 02 03]   [s2  s6  s10 s14]
         * [03 01 01 02]   [s3  s7  s11 s15]
         */
        for (i = 0; i < AES_BLOCK_SIZE; i+=4)  {
            t = state[i] ^ state[i+1] ^ state[i+2] ^ state[i+3];
            
            ciphertext[i]   = Multiply(2, state[i]   ^ state[i+1]) ^ state[i]   ^ t;
            
            ciphertext[i+1] = Multiply(2, state[i+1] ^ state[i+2]) ^ state[i+1] ^ t;
            
            ciphertext[i+2] = Multiply(2, state[i+2] ^ state[i+3]) ^ state[i+2] ^ t;
            
            ciphertext[i+3] = Multiply(2, state[i+3] ^ state[i]  ) ^ state[i+3] ^ t;
        }

        // AddRoundKey
        for ( i = 0; i < AES_BLOCK_SIZE; ++i ) {
            ciphertext[i] = Add(ciphertext[i], *roundkeys++);
        }

    }
    
    // last round
    for (i = 0; i < AES_BLOCK_SIZE; ++i) {
        ciphertext[i] = get_sbox_value(ciphertext[i]);
    }
    shift_rows(ciphertext);
    for ( i = 0; i < AES_BLOCK_SIZE; ++i ) {
        ciphertext[i] = Add(ciphertext[i], *roundkeys++);
    }

}


void aes_decrypt_128(uint8_t *roundkeys, uint8_t *ciphertext, uint8_t *plaintext) {

    uint8_t state[AES_BLOCK_SIZE];
    uint8_t t, u, v;
    uint8_t i, j;

    roundkeys += 160; //160, 192, 224

    // first round
    for ( i = 0; i < AES_BLOCK_SIZE; ++i ) {
        plaintext[i] = Add(ciphertext[i], roundkeys[i]);
    }
    
    roundkeys -= 16;
    inv_shift_rows(plaintext);
    
    for (i = 0; i < AES_BLOCK_SIZE; ++i) {
        plaintext[i] = get_inv_sbox_value(plaintext[i]);
    }

    for (j = 1; j < AES_ROUNDS; ++j) {
        
        // Inverse AddRoundKey
        for ( i = 0; i < AES_BLOCK_SIZE; ++i ) {
            state[i] = Add(plaintext[i], roundkeys[i]);
        }
        
        /*
         * Inverse MixColumns
         * [0e 0b 0d 09]   [s0  s4  s8  s12]
         * [09 0e 0b 0d] . [s1  s5  s9  s13]
         * [0d 09 0e 0b]   [s2  s6  s10 s14]
         * [0b 0d 09 0e]   [s3  s7  s11 s15]
         */
        for (i = 0; i < AES_BLOCK_SIZE; i+=4) {
            t = state[i] ^ state[i+1] ^ state[i+2] ^ state[i+3];
            plaintext[i]   = t ^ state[i]   ^ Multiply(2, (state[i]   ^ state[i+1]));
            plaintext[i+1] = t ^ state[i+1] ^ Multiply(2, (state[i+1] ^ state[i+2]));
            plaintext[i+2] = t ^ state[i+2] ^ Multiply(2, (state[i+2] ^ state[i+3]));
            plaintext[i+3] = t ^ state[i+3] ^ Multiply(2, (state[i+3] ^ state[i]));
            u = Multiply(2, Multiply(2, (state[i]   ^ state[i+2])) );
            v = Multiply(2, Multiply(2, (state[i+1] ^ state[i+3])) );
            t = Multiply(2, (u ^ v));
            plaintext[i]   ^= t ^ u;
            plaintext[i+1] ^= t ^ v;
            plaintext[i+2] ^= t ^ u;
            plaintext[i+3] ^= t ^ v;
        }
        
        // Inverse ShiftRows
        inv_shift_rows(plaintext);
        
        // Inverse SubBytes
        for (i = 0; i < AES_BLOCK_SIZE; ++i) {
            plaintext[i] = get_inv_sbox_value(plaintext[i]);
        }

        roundkeys -= 16;

    }

    // last AddRoundKey
    for ( i = 0; i < AES_BLOCK_SIZE; ++i ) {
        plaintext[i] = Add(plaintext[i], roundkeys[i]);
    }

}

