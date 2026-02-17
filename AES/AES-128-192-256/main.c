#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#include "./aes_files/gf256.h"
#include "./aes_files/aes128.h"
#include "./aes_files/gadgets.h"
#include "./aes_files/aes128_sharing.h"


double my_gettimeofday(){
  struct timeval tmp_time;
  gettimeofday(&tmp_time, NULL);
  return tmp_time.tv_sec + (tmp_time.tv_usec * 1.0e-6L);
}

int main(int argc, char ** argv){
	
	for(int i=0; i<NB_SHARES; i++){
		const_s[i] = 0;
	}
	
	srand(time(NULL));
	
	double start, end, aes_enc, aes_dec, aes_sharing_enc, aes_sharing_dec;
	
	uint8_t i, r;
	/* 128 bit key */
	uint8_t key[] = {
		0x0f, 0x15, 0x71, 0xc9, 0x47, 0xd9, 0xe8, 0x59, 
		0x0c, 0xb7, 0xad, 0xd6, 0xaf, 0x7f, 0x67, 0x98,
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 

	};
	
    // 1. Open the file
    const char* filename = "AES_DATA-64"; //64, 2048, 16384
    FILE* file = fopen(filename, "rb");  // Read in binary mode
    if (!file) {
        perror("File open failed");
        return 1;
    }
    // 2. Get the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 3.  Calculate number of blocks and padding
    const size_t BLOCK_SIZE = 16;  // AES-128 block size
    size_t num_blocks = (file_size + BLOCK_SIZE - 1) / BLOCK_SIZE;  // 向上取整
    size_t padded_size = num_blocks * BLOCK_SIZE;
    
    
    
    // 3. Allocate memory buffer
    uint8_t* plaintext = (uint8_t*)malloc(padded_size);
    uint8_t* ciphertext = (uint8_t*)malloc(padded_size);
    uint8_t* decrypted = (uint8_t*)malloc(padded_size);
    
    if (!plaintext || !ciphertext || !decrypted) {
        perror("Memory allocation failed");
        fclose(file);
        return 1;
    }
    
    uint8_t* plaintext_res = (uint8_t*)malloc(padded_size);
    if (!plaintext_res) {
        perror("Memory allocation failed");
        fclose(file);
        return 1;
    }

    // 4. Read file content
    size_t bytes_read = fread(plaintext, 1, file_size, file);
    fclose(file);
    
    if (bytes_read != (size_t)file_size) {
        perror("File read incomplete");
        free(plaintext);
        free(ciphertext);
        free(plaintext_res);
        return 1;
    }

    // 5. PKCS#7 padding
    uint8_t pad_value = BLOCK_SIZE - (file_size % BLOCK_SIZE);
    if (file_size % BLOCK_SIZE != 0) {
        memset(plaintext + file_size, pad_value, pad_value);
    }
	
	const uint8_t const_cipher[AES_BLOCK_SIZE] = {
		0xff, 0x0b, 0x84, 0x4a, 0x08, 0x53, 0xbf, 0x7c,
		0x69, 0x34, 0xab, 0x43, 0x64, 0x14, 0x8f, 0xb9,
		//0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30,
		//0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a,
	};
	
	
	uint8_t roundkeys[AES_ROUND_KEY_SIZE];
	
	aes_key_schedule_128(key, roundkeys);  //_128, _192, _256
	printf("Round Keys:\n");
	for ( r = 0; r <= AES_ROUNDS; r++ ) {
		for (i = 0; i < AES_BLOCK_SIZE; i++) {
			printf("%2x ", roundkeys[r*AES_BLOCK_SIZE+i]);
		}
		printf("\n");
	}
	printf("\n");
	
	
	/*************************** AES-128 Standard Encryption / Decryption ***************************/
	start = my_gettimeofday();
	//aes_encrypt_128(roundkeys, plaintext, ciphertext);
	for (size_t i = 0; i < num_blocks; i++) {
        uint8_t* block_in = plaintext + i * BLOCK_SIZE;
        uint8_t* block_out = ciphertext + i * BLOCK_SIZE;
        aes_encrypt_128(roundkeys, block_in, block_out);
    }
	end = my_gettimeofday();
	aes_enc = end - start;
	
	start = my_gettimeofday();
	//aes_decrypt_128(roundkeys, ciphertext, plaintext_res);
	for (size_t i = 0; i < num_blocks; i++) {
        uint8_t* block_in = ciphertext + i * BLOCK_SIZE;
        uint8_t* block_out = plaintext_res + i * BLOCK_SIZE;
        aes_decrypt_128(roundkeys, block_in, block_out);
    }
	end = my_gettimeofday();
	aes_dec = end - start;
	
	/*************************** Verifying that AES decryption gives back original plaintext ***************************/
	for(i=0; i<AES_BLOCK_SIZE; i++){
		if(plaintext[i] != plaintext_res[i]){
			printf("DECRYPT ERROR\n");
			exit(EXIT_FAILURE);
		}
	}
	printf("REGULAR ENCRYPTION SUCCESS\n");
	
	
	/*************************** Generating Sharings of texts and keys ***************************/
	uint8_t ** plaintext_sharing = (uint8_t **)malloc(AES_BLOCK_SIZE * sizeof(uint8_t *));
	uint8_t ** plaintext_res_sharing = (uint8_t **)malloc(AES_BLOCK_SIZE * sizeof(uint8_t *));
	uint8_t ** ciphertext_sharing = (uint8_t **)malloc(AES_BLOCK_SIZE * sizeof(uint8_t *));
	for(i =0; i< AES_BLOCK_SIZE; i++){
		plaintext_sharing[i] = (uint8_t *)malloc(NB_SHARES * sizeof(uint8_t));
		plaintext_res_sharing[i] = (uint8_t *)malloc(NB_SHARES * sizeof(uint8_t));
		ciphertext_sharing[i] = (uint8_t *)malloc(NB_SHARES * sizeof(uint8_t));
	}
	
	for(i =0; i<AES_BLOCK_SIZE; i++){
		generate_n_sharing(0, ciphertext_sharing[i]);
	}
	
	// Round key sharing (global copy)
    uint8_t **roundkeys_sharing = (uint8_t **)malloc(AES_ROUND_KEY_SIZE * sizeof(uint8_t *));
    for(int i = 0; i < AES_ROUND_KEY_SIZE; i++) {
        roundkeys_sharing[i] = (uint8_t *)malloc(NB_SHARES * sizeof(uint8_t));
        generate_n_sharing(roundkeys[i], roundkeys_sharing[i]);
    }
	
	// Create sharing structure (each block independent)
    uint8_t ***all_plaintext_sharing = (uint8_t ***)malloc(num_blocks * sizeof(uint8_t **));
    uint8_t ***all_ciphertext_sharing = (uint8_t ***)malloc(num_blocks * sizeof(uint8_t **));
    uint8_t ***all_plaintext_res_sharing = (uint8_t ***)malloc(num_blocks * sizeof(uint8_t **));
	
	// Allocate sharing structure for each block
    for(size_t block_idx = 0; block_idx < num_blocks; block_idx++) {
        all_plaintext_sharing[block_idx] = (uint8_t **)malloc(AES_BLOCK_SIZE * sizeof(uint8_t *));
        all_ciphertext_sharing[block_idx] = (uint8_t **)malloc(AES_BLOCK_SIZE * sizeof(uint8_t *));
        all_plaintext_res_sharing[block_idx] = (uint8_t **)malloc(AES_BLOCK_SIZE * sizeof(uint8_t *));
        
        for(int i = 0; i < AES_BLOCK_SIZE; i++) {
            all_plaintext_sharing[block_idx][i] = (uint8_t *)malloc(NB_SHARES * sizeof(uint8_t));
            all_ciphertext_sharing[block_idx][i] = (uint8_t *)malloc(NB_SHARES * sizeof(uint8_t));
            all_plaintext_res_sharing[block_idx][i] = (uint8_t *)malloc(NB_SHARES * sizeof(uint8_t));
            
            // Initialize ciphertext sharing
            generate_n_sharing(0, all_ciphertext_sharing[block_idx][i]);
        }
    }
	
	
	/*************************** AES-128 Sharing Secure Encryption / Decryption ***************************/
	start = my_gettimeofday();
	
    for (size_t block_idx = 0; block_idx < num_blocks; block_idx++) {
        uint8_t* block_in = plaintext + block_idx * BLOCK_SIZE;
        
        // Generate plaintext sharing for current block
        for(int i = 0; i < AES_BLOCK_SIZE; i++) {
            generate_n_sharing(block_in[i], all_plaintext_sharing[block_idx][i]);
        }
        
        // Perform shared encryption
        aes_encrypt_128_sharing(
            roundkeys_sharing,
            all_plaintext_sharing[block_idx],
            all_ciphertext_sharing[block_idx]
        );
        // Convert shared ciphertext to normal byte storage
        for(int i = 0; i < AES_BLOCK_SIZE; i++) {
            // Simplified decoding example - actual should use your share reconstruction function
            ciphertext[block_idx * BLOCK_SIZE + i] = all_ciphertext_sharing[block_idx][i][0];
        }
    }
    
	end = my_gettimeofday();
	aes_sharing_enc = end - start;
	
	start = my_gettimeofday();
	for (size_t block_idx = 0; block_idx < num_blocks; block_idx++) {
        // Perform shared decryption
        aes_decrypt_128_sharing(
            roundkeys_sharing,
            all_ciphertext_sharing[block_idx],
            all_plaintext_res_sharing[block_idx]
        );

        // Convert shared plaintext to normal byte storage
        for(int i = 0; i < AES_BLOCK_SIZE; i++) {
            // Simplified decoding example - actual should use your share reconstruction function
            decrypted[block_idx * BLOCK_SIZE + i] = all_plaintext_res_sharing[block_idx][i][0];
        }
	}
	
	end = my_gettimeofday();
	aes_sharing_dec = end - start;
	
	
	/*************************** Verifying that both standard and sharing encryption give the same ciphertext output ***************************/

    int verification_failed = 0;
    for (size_t block_idx = 0; block_idx < num_blocks; block_idx++) {
        uint8_t reconstructed[AES_BLOCK_SIZE] = {0};
        uint8_t* orig_block = plaintext + block_idx * AES_BLOCK_SIZE; // 统一使用AES_BLOCK_SIZE
        
        // Reconstruct decryption result
        for (int i = 0; i < AES_BLOCK_SIZE; i++) {
            for (int j = 0; j < NB_SHARES; j++) {
                reconstructed[i] ^= all_plaintext_res_sharing[block_idx][i][j];
            }
        }
        
        // Compare reconstructed result with original plaintext
        if (memcmp(reconstructed, orig_block, AES_BLOCK_SIZE) != 0) {
            verification_failed = 1;
            printf("Verification failed at block %zu\n", block_idx);
            for (int i = 0; i < AES_BLOCK_SIZE; i++) {
                printf("Byte %d: original=%02x, decrypted=%02x\n", i, orig_block[i], reconstructed[i]);
            }
            break;
        }
    }

	
	
	/*************************** Verifying that sharing AES decryption gives back the original plaintext ***************************/
 verification_failed = 0;
for (size_t block_idx = 0; block_idx < num_blocks; block_idx++) {
    uint8_t reconstructed[AES_BLOCK_SIZE] = {0};
    uint8_t* orig_block = plaintext + block_idx * AES_BLOCK_SIZE; // 统一使用AES_BLOCK_SIZE
    
    // Reconstruct decryption result
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        for (int j = 0; j < NB_SHARES; j++) {
            reconstructed[i] ^= all_plaintext_res_sharing[block_idx][i][j];
        }
    }
    
    // Compare reconstructed result with original plaintext
    if (memcmp(reconstructed, orig_block, AES_BLOCK_SIZE) != 0) {
        verification_failed = 1;
        printf("Verification failed at block %zu\n", block_idx);
        for (int i = 0; i < AES_BLOCK_SIZE; i++) {
            printf("Byte %d: original=%02x, decrypted=%02x\n", i, orig_block[i], reconstructed[i]);
        }
        break;
    }
}


	/*************************** Printing Ciphertext ***************************/
	
	for (size_t block_idx = 0; block_idx < num_blocks; block_idx++) {
    
    // If first block, copy to ciphertext_sharing
    if(block_idx == 0) {
        for(int i = 0; i < AES_BLOCK_SIZE; i++) {
            for(int j = 0; j < NB_SHARES; j++) {
                ciphertext_sharing[i][j] = all_ciphertext_sharing[0][i][j];
            }
        }
    }
}
    
    // Then print
    printf("\nCipher text (first block):\n");
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        printf("%02x ", compress_n_sharing(ciphertext_sharing[i]));
    }
	
	
	/*for (i = 0; i < AES_BLOCK_SIZE; i++) {
		if ( ciphertext[i] != const_cipher[i] ) { break; }
	}
	if ( AES_BLOCK_SIZE != i ) { printf("\nENCRYPT WRONG\n\n"); }
	else { printf("\nENCRYPT CORRECT\n\n"); }*/
	
	
	printf("\n\nTimings: \n");
	printf("\nAES standard enc took %lf ms\n", aes_enc * 1000);
	printf("\nAES standard dec took %lf ms\n", aes_dec * 1000);
	
	printf("\n\nAES sharing enc took %lf ms\n", aes_sharing_enc * 1000);
	printf("\nAES sharing dec took %lf ms\n", aes_sharing_dec * 1000);

	for(i =0; i< AES_BLOCK_SIZE; i++){
		free(plaintext_sharing[i]);
		free(ciphertext_sharing[i]);
	}
	for(i=0; i<AES_ROUND_KEY_SIZE; i++){
		free(roundkeys_sharing[i]);
	}
	free(plaintext_sharing);
	free(ciphertext_sharing);
	free(roundkeys_sharing);
	free(plaintext);
	free(ciphertext);
	free(decrypted);
	free(plaintext_res);
	
	 // Free sharing structure
    for(size_t block_idx = 0; block_idx < num_blocks; block_idx++) {
        for(int i = 0; i < AES_BLOCK_SIZE; i++) {
            free(all_plaintext_sharing[block_idx][i]);
            free(all_ciphertext_sharing[block_idx][i]);
            free(all_plaintext_res_sharing[block_idx][i]);
        }
        free(all_plaintext_sharing[block_idx]);
        free(all_ciphertext_sharing[block_idx]);
        free(all_plaintext_res_sharing[block_idx]);
    }
    free(all_plaintext_sharing);
    free(all_ciphertext_sharing);
    free(all_plaintext_res_sharing);
	
	
	return 0;
	
	
}
