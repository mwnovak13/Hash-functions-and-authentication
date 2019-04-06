
void main(){
	
	//Read encrypted message from Alice
	memset(msgbuffer, 0, sizeof(msgbuffer));
	read(sock, msgbuffer, 2048);

	//Decrpted the messgae from Alice
	AES_init_ctx(&ctx, aeskey);
	AES_decrypt(&ctx, msgbuffer);
	printf("Message recieved from Alice is - %s \n", msgbuffer);
	//Create variables Hash, Comphash and MessageNoHash
	char hash[2048];
	char comphash[2048];
	char messageNoHash[2048];
	//Loop through msgbuffer, only for the first 12 bits in the array. Store into MessageNoHash
	int i;
	for (i = 0; i < 12; i++)
	{
		msgbuffer[i] = messageNoHash;
	}
	//Loop through mesgbuffer again, but only from 12 the size of mesgbuffer, store into Hash
	for (i = 12; i < sizeof(msgbuffer); i++)
	{
		msgbuffer[i] = hash;
	}
//Decrypt Hash*********************************************************************************************************
	mpz_t ptexth;
	mpz_t ctexth;
	mpz_init(ptexth); // plain text
	mpz_init(ctexth); // cipher text

	//Place hash into ctexth
	mpz_import(ctexth, strlen(hash), 1, 1, 0, 0, hash);
	gmp_printf("\nThe ctext of messageNoHash is %Zd \n", ctexth);
	//decrypting the cipher text to plain text 
	//formula - plaintext = ciphertext power e mod n
	mpz_powm(ptexth, ctexth, e, n);
	gmp_printf("\nThe ptext of messageNoHash is %Zd \n", ptexth);

//Decrypt Message*******************************************************************************************************
	//Place messageNoHash into ctext
	mpz_import(ctext, strlen(messageNoHash), 1, 1, 0, 0, messageNoHash);
	gmp_printf("\nThe ctext of messageNoHash is %Zd \n", ctext);
	//decrypting the cipher text to plain text 
	//formula - plaintext = ciphertext power e mod n
	mpz_powm(ptext, ctext, e, n);
	gmp_printf("\nThe ptext of messageNoHash is %Zd \n", ptext);
	
	
	
	
	
	//Compare Comphash and hash.
	if`(comphash = hash) {
	printf("\nThe Hash matches!");
}
	else {
		printf("\nThe Hash does NOT match! This message may have been tampered with.");
	}




//****************************************************************************************************************************************

	mpz_import(ctext, strlen(buffer), 1, 1, 0, 0, buffer);
	gmp_printf("\nThe ctext of RSA key is %Zd \n", ctext);

	//decrypting the cipher text to plain text 
	//formula - plaintext = ciphertext power d mod n
	mpz_powm(ptext, ctext, d, n);
	gmp_printf("\nThe ptext of RSA key is %Zd \n", ptext);

	//Export the plaintext into array aeskey
	memset(aeskey, 0, sizeof(aeskey));
	mpz_export(aeskey, NULL, 1, 1, 0, 0, ptext);

}
