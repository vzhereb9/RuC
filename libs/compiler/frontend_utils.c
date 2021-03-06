/*
 *	Copyright 2019 Andrey Terekhov
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

#include "codegen.h"
#include "codes.h"
#include "extdecl.h"
#include "global.h"
#include "keywords.h"
#include "scanner.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__APPLE__) || defined(__linux__)
	#include <sys/stat.h>
	#include <sys/types.h>
#endif


/** Make executable actually executable on best-effort basis (if possible) */
static void make_executable(const char *path)
{
#if defined(__APPLE__) || defined(__linux__)
	struct stat stat_buf;

	if (stat(path, &stat_buf) != 0)
	{
		return;
	}

	chmod(path, stat_buf.st_mode | S_IXUSR);
#endif
}

/** Занесение ключевых слов в reprtab */
void read_keywords(compiler_context *context)
{
	size_t len = strlen(keywords_txt);
	char *keywords = malloc(len + 1);

	if (keywords == NULL)
	{
		exit(-1);
	}

	memcpy(keywords, keywords_txt, len + 1);

	compiler_context_attach_io(context, keywords, IO_TYPE_INPUT, IO_SOURCE_MEM);

	context->keywordsnum = 1;
	getnext(context);
	nextch(context);
	while (scan(context) != LEOF)
	{
		; // чтение ключевых слов
	}

	compiler_context_detach_io(context, IO_TYPE_INPUT);
}

/** Вывод таблиц и дерева */
void output_tables_and_tree(compiler_context *context, const char *path)
{
	compiler_context_attach_io(context, path, IO_TYPE_OUTPUT, IO_SOURCE_FILE);

	getnext(context);
	nextch(context);
	context->next = scan(context);

	ext_decl(context); // генерация дерева

	context->lines[context->line + 1] = context->charnum;
	tablesandtree(context);
	compiler_context_detach_io(context, IO_TYPE_OUTPUT);
}

/** Генерация кодов */
void output_codes(compiler_context *context, const char *path)
{
	compiler_context_attach_io(context, path, IO_TYPE_OUTPUT, IO_SOURCE_FILE);
	codegen(context);
	tablesandcode(context);
	compiler_context_detach_io(context, IO_TYPE_OUTPUT);
}

/** Вывод таблиц в файл */
void output_export(compiler_context *context, const char *path)
{
	int i;

	compiler_context_attach_io(context, path, IO_TYPE_OUTPUT, IO_SOURCE_FILE);
	printer_printf(&context->output_options, "#!/usr/bin/ruc-vm\n");

	printer_printf(&context->output_options, "%i %i %i %i %i %i %i\n", context->pc, context->funcnum, context->id,
				   REPRTAB_LEN, context->md, context->maxdisplg, context->wasmain);

	for (i = 0; i < context->pc; i++)
	{
		printer_printf(&context->output_options, "%i ", context->mem[i]);
	}
	printer_printf(&context->output_options, "\n");

	for (i = 0; i < context->funcnum; i++)
	{
		printer_printf(&context->output_options, "%i ", context->functions[i]);
	}
	printer_printf(&context->output_options, "\n");

	for (i = 0; i < context->id; i++)
	{
		printer_printf(&context->output_options, "%i ", context->identab[i]);
	}
	printer_printf(&context->output_options, "\n");

	for (i = 0; i < REPRTAB_LEN; i++)
	{
		printer_printf(&context->output_options, "%i ", REPRTAB[i]);
	}

	for (i = 0; i < context->md; i++)
	{
		printer_printf(&context->output_options, "%i ", context->modetab[i]);
	}
	printer_printf(&context->output_options, "\n");

	compiler_context_detach_io(context, IO_TYPE_OUTPUT);

	make_executable(path);
}
