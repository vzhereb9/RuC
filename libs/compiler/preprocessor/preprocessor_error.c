/*
 *	Copyright 2018 Andrey Terekhov, Egor Anikin
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *		http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 */

#include "preprocessor_error.h"
#include "constants.h"
#include "context.h"
#include "context_var.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void printf_character(int wchar)
{
	if (wchar < 128)
	{
		printf("%c", wchar);
	}
	else
	{
		unsigned char first = (wchar >> 6) | /*0b11000000*/ 0xC0;
		unsigned char second = (wchar & /*0b111111*/ 0x3F) | /*0b10000000*/ 0x80;

		printf("%c%c", first, second);
	}
}

void a_erorr(int i)
{
	printf("не реализованная ошибка № %d\n", i);

	exit(2);
}

void m_error(int ernum, compiler_context *c_context)
{
	int i;

	// tablesandtree();
	printf("line %i) ", c_context->mline);
	for (i = c_context->mlines[c_context->mline]; i < c_context->m_charnum; i++)
	{
		printf_character(c_context->before_source[i]);
	}

	printf("\n");
	switch (ernum)
	{
		case after_preproces_words_must_be_space:
			printf("После комады препроцессора должен идти перенос строки\n");
			break;
		case after_ident_must_be_space:
			printf("После индентификатора должен идти ' ' \n");
			break;
		case ident_begins_with_letters1:
			printf("Идентификатор должен начинаться с буквы \n");
			break;
		case must_be_endif:
			printf("Условный оператор препроцессора должен заканчиваться '#ENDIF' \n");
			break;
		case dont_elif:
			printf("В этом типе условного оператора не может использоваться '#ELIF' \n");
			break;
		case preproces_words_not_exist:
			printf("В препроцессоре не сущетвует написанной команды\n");
			break;
		case not_enough_param:
			printf("У этого идентификатора меньше параметров\n");
			break;
		case functionid_begins_with_letters:
			printf("Идентификатор с параметрами должн начинаться с буквы\n");
			break;
		case functions_cannot_be_changed:
			printf("Идентификатор с параметрами нельзя переопределять\n");
			break;
		case after_functionid_must_be_comma:
			printf("После идентификатора в функции должны быть ')' или ',' потом ' ' \n");
			break;
		case stalpe:
			printf("В функции аргументы должны быть описаны через запятую, в скобках\n");
			break;
		case not_relis_if:
			printf("if ещё не реализован");
			break;
		case befor_endif:
			printf("Перед '#ENDIF' должен стоять условный оператор препроцессора\n");
			break;
		case repeat_ident:
			printf("Этот идентификатор препроцессора уже используется\n");
			break;
		case ident_not_exist:
			printf("Данный идентификатор препроцессора не существует\n");
			break;
		case comm_not_ended:
			printf("Комментарий, начавшийся с /* , не закрыт\n");
			break;
		case not_enough_param2:
			printf("У этой функции больше параметров\n");
			break;
		case not_end_fail_define:
			printf("Фаил не может закончится до окончания комады '#DEFINE' постате перенос строки\n");
			break;
		case scob_not_clous:
			printf("Количество открывающих скобок не соотвеcтвует числу закрывающих\n");
			break;
		case after_eval_must_be_ckob:
			printf("Сразу после команды '#EVAL' должен быть символ '('\n");
			break;
		case too_many_nuber:
			printf("Слишком большое число\n");
			break;
		case must_be_digit_after_exp1:
			printf("после експоненты должно быть число\n");
			break;
		case not_arithmetic_operations:
			printf("Все арефметические операции должны быть внутри команды '#EVAL()'\n");
			break;
		case not_logical_operations:
			printf("Внутри команды '#EVAL()' не должно быть логических операций\n");
			break;
		default:;
	}

	exit(2);
}
