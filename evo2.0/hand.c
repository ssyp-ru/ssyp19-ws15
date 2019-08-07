#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "distributor.h"
#include "hand.h"

hands_t* create_new_hand(){
	hands_t* new_hand = (hands_t*)malloc(sizeof(hands_t));
	new_hand->hand = (dcard_t*)malloc(sizeof(dcard_t) * 6);
	new_hand->used = 0;
	new_hand->alloced = 6;
	return new_hand;
}

trash_t* create_new_trash(){
	trash_t* new_trash = (trash_t*)malloc(sizeof(trash_t));
	new_trash->trash = (tcard_t*)malloc(sizeof(tcard_t));
	new_trash->used = 0;
	new_trash->alloced = 1;
	return new_trash;
}

void resize_hand(hands_t* hand){
	dcard_t* tmp = (dcard_t*)malloc(sizeof(dcard_t) * hand->alloced * 2);
	for (int i = 0; i < hand->used; i++){
		tmp[i].f_name = hand->hand[i].f_name;
		tmp[i].s_name = hand->hand[i].s_name;
		tmp[i].weights = hand->hand[i].weights;
	}
	free(hand->hand);
	hand->hand = tmp;
	hand->alloced *= 2;
}

void resize_trash(trash_t* trash){
	tcard_t* tmp = (tcard_t*)malloc(sizeof(tcard_t) * trash->alloced * 2);
	for (int i = 0; i < trash->used; i++)
		tmp[i].name = trash->trash[i].name;
	free(trash->trash);
	trash->trash = tmp;
	trash->alloced *= 2;
}

void add_card_to_hand(hands_t* hand, char* f_name, char* s_name, uint16_t* weights){
	if (hand->alloced - hand->used == 0)
		resize_hand(hand);
	hand->hand[hand->used].f_name = f_name;
	hand->hand[hand->used].s_name = s_name;
	hand->hand[hand->used].weights = weights;
	hand->used++;
}

void add_card_to_trash(trash_t* trash, char* name){
	if (trash->alloced - trash->used == 0)
		resize_trash(trash);
	trash->trash[trash->used].name = name;
	trash->used++;
}

void delete_card_from_hand(hands_t* hand, uint16_t num){
	if (hand->used >= num){
		for (int i = 0; i < hand->used - num - 1; i++){
			hand->hand[num + i].f_name = hand->hand[num + i + 1].f_name;
			hand->hand[num + i].s_name = hand->hand[num + i + 1].s_name;
		}
		hand->used--;
	}
}

void delete_hand(hands_t* hand){
	//for (uint16_t i = 0; i < hand->used; i++)
	//	delete_card_from_hand(hand, i);
	free(hand->hand);
	free(hand);
}

void delete_trash(trash_t* trash){
	free(trash->trash);
	free(trash);
}
