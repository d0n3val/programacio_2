
#ifndef BASE64_H
#define BASE64_H

static const char b64enc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz" "0123456789" "+/";

char* b64_encode(const char *source, unsigned int length) {
	unsigned int i, mlen, r_pos;
	unsigned short dif, j;
	unsigned int frame = 0;
	char out[5];
	char *res;
	
	mlen = 4 * length/3 + 1; /* +1 : returns a null-terminated string */
	if (length%3) {
		mlen += 4;
	}
	
	res = (char*) malloc(mlen);
	if (!res) {
		LOG("Base64: could not allocate memory");
		return NULL;
	}
	res[mlen-1] = '\0';
	out[4] = '\0';
	
	for (i=0; i<length; i+=3) {
		/*frame = 0; clean frame not needed because '>>' inserts '0' */
		dif = (length-i)/3 ? 3 : (length-i)%3; /* number of byte to read */
		for (j=0; j<dif; j++) {
			memcpy(((char*)&frame)+2-j, source+i+j, 1); /* copy 3 bytes in reverse order */
		}
		/*
		now 3 cases :
		. 3B red => 4chars
		. 2B red => 3chars + "="
		. 1B red => 2chars + "=="
		*/
		for (j=0; j<dif+1; j++) {
			out[j] = (char)((frame & 0xFC0000) >> 18); /* first 6 bits */
			out[j] = b64enc[(int)out[j]];
			frame = frame << 6; /* next 6b word */
		}
		if (dif == 1) {
			out[2] = out [3] = '=';
		} else if (dif == 2) {
			out [3] = '=';
		}
		r_pos = (i/3)*4;
		strncpy_s(res+r_pos, mlen, out, mlen);
	}
	return res;
}

static char b64_value(char c) {
	if (c>='A' && c<='Z') {
		return c - 'A';
	} else if (c>='a' && c<='z') {
		return c - 'a' + 26;
	} else if (c>='0' && c<='9') {
		return c - '0' + 52;
	} else if (c=='+') {
		return 62;
	} else if (c=='/') {
		return 63;
	} else if (c=='=') {
		return 0;
	}
	return -1;
}

char* b64_decode(const char *source, unsigned int *rlength) { /* NULL terminated string */
	char *res, v;
	short j;
	unsigned int i;
	unsigned int in = 0;
	unsigned int src_len = strlen(source);
	
	if (!source) {
		LOG("Base64: invalid argument: source is NULL");
		return NULL;
	}
	
	if (src_len%4) {
		LOG("Base64: invalid source");
		return NULL; /* invalid source */
	}
	
	*rlength = (src_len/4)*3;
	res = (char*) malloc(*rlength);
	if (!res) {
		LOG("Base64: could not allocate memory");
		return NULL;
	}
	
	for (i=0; i<src_len; i+=4) {
		in = 0;
		
		for (j=0; j<4; j++) {
			v = b64_value(source[i+j]);
			if (v == -1) {
				LOG("Base64: invalid char '%c' in source", source[i+j]);
				goto cleanup;
			}
			in = in << 6;
			in += v; /* add 6b */
		}
		for (j=0; j<3; j++) {
			memcpy(res+(i/4)*3+j, ((char*)&in)+2-j, 1); /* copy 3 bytes in reverse order */
		}
	}
	
	if (source[src_len-1] == '=') {
		(*rlength)--;
	}
	if (source[src_len-2] == '=') {
		(*rlength)--;
	}
	
	return res;
	
cleanup:
	free(res);
	return NULL;
}
#endif