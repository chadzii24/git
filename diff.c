static int diff_indent_heuristic; /* experimental */
	if (git_diff_heuristic_config(var, value, cb) < 0)
		return -1;

static void emit_line_0(struct diff_options *o, const char *set, const char *reset,
			int first, const char *line, int len)
	int nofirst;
	fputs(diff_line_prefix(o), file);

	if (len == 0) {
		has_trailing_newline = (first == '\n');
		has_trailing_carriage_return = (!has_trailing_newline &&
						(first == '\r'));
		nofirst = has_trailing_newline || has_trailing_carriage_return;
	} else {
		has_trailing_newline = (len > 0 && line[len-1] == '\n');
		has_trailing_carriage_return = (len > 0 && line[len-1] == '\r');
		nofirst = 0;
	if (len || !nofirst) {
		fputs(set, file);
		if (!nofirst)
			fputc(first, file);
		fwrite(line, len, 1, file);
		fputs(reset, file);
	}
	if (has_trailing_carriage_return)
		fputc('\r', file);
	if (has_trailing_newline)
		fputc('\n', file);
static void emit_line(struct diff_options *o, const char *set, const char *reset,
		      const char *line, int len)
	emit_line_0(o, set, reset, line[0], line+1, len-1);
		emit_line_0(ecbdata->opt, set, reset, sign, line, len);
		emit_line_0(ecbdata->opt, ws, reset, sign, line, len);
		emit_line_0(ecbdata->opt, set, reset, sign, "", 0);
		ws_check_emit(line, len, ecbdata->ws_rule,
			      ecbdata->opt->file, set, reset, ws);
		emit_line(ecbdata->opt, context, reset, line, len);
	emit_line(ecbdata->opt, "", "", msgbuf.buf, msgbuf.len);
static void print_line_count(FILE *file, int count)
		fprintf(file, "0,0");
		fprintf(file, "1");
		fprintf(file, "1,%d", count);
	const char *endp = NULL;
	static const char *nneof = " No newline at end of file\n";
		endp = memchr(data, '\n', size);
		len = endp ? (endp - data + 1) : size;
	if (!endp) {
		putc('\n', ecb->opt->file);
		emit_line_0(ecb->opt, context, reset, '\\',
	const char *line_prefix = diff_line_prefix(o);
	fprintf(o->file,
		"%s%s--- %s%s%s\n%s%s+++ %s%s%s\n%s%s@@ -",
		line_prefix, metainfo, a_name.buf, name_a_tab, reset,
		line_prefix, metainfo, b_name.buf, name_b_tab, reset,
		line_prefix, fraginfo);
		print_line_count(o->file, lc_a);
		fprintf(o->file, "?,?");
	fprintf(o->file, " +");
	print_line_count(o->file, lc_b);
	fprintf(o->file, " @@%s\n", reset);
static int fn_out_diff_words_write_helper(FILE *fp,
					  size_t count, const char *buf,
					  const char *line_prefix)
			fputs(line_prefix, fp);
			if (st_el->color && fputs(st_el->color, fp) < 0)
				return -1;
			if (fputs(st_el->prefix, fp) < 0 ||
			    fwrite(buf, p ? p - buf : count, 1, fp) != 1 ||
			    fputs(st_el->suffix, fp) < 0)
				return -1;
			if (st_el->color && *st_el->color
			    && fputs(GIT_COLOR_RESET, fp) < 0)
				return -1;
			return 0;
		if (fputs(newline, fp) < 0)
			return -1;
	const char *line_prefix;
	line_prefix = diff_line_prefix(opt);
	if (color_words_output_graph_prefix(diff_words)) {
		fputs(line_prefix, diff_words->opt->file);
	}
		fn_out_diff_words_write_helper(diff_words->opt->file,
				diff_words->current_plus, line_prefix);
		if (*(plus_begin - 1) == '\n')
			fputs(line_prefix, diff_words->opt->file);
		fn_out_diff_words_write_helper(diff_words->opt->file,
				minus_end - minus_begin, minus_begin,
				line_prefix);
		fn_out_diff_words_write_helper(diff_words->opt->file,
				plus_end - plus_begin, plus_begin,
				line_prefix);
	const char *line_prefix;

	line_prefix = diff_line_prefix(opt);
		fputs(line_prefix, diff_words->opt->file);
		fn_out_diff_words_write_helper(diff_words->opt->file,
			diff_words->minus.text.ptr, line_prefix);
			fputs(line_prefix, diff_words->opt->file);
		fn_out_diff_words_write_helper(diff_words->opt->file,
			- diff_words->current_plus, diff_words->current_plus,
			line_prefix);
	const char *line_prefix = diff_line_prefix(o);
		fprintf(o->file, "%s", ecbdata->header->buf);
		strbuf_reset(ecbdata->header);


		fprintf(o->file, "%s%s--- %s%s%s\n",
			line_prefix, meta, ecbdata->label_path[0], reset, name_a_tab);
		fprintf(o->file, "%s%s+++ %s%s%s\n",
			line_prefix, meta, ecbdata->label_path[1], reset, name_b_tab);
		if (line[len-1] != '\n')
			putc('\n', o->file);
			emit_line(o, context, reset, line, len);
			fputs("~\n", o->file);
			emit_line(o, context, reset, line, len);
			  reset, line, len);
static void show_name(FILE *file,
	fprintf(file, " %s%-*s |", prefix, len, name);
static void show_graph(FILE *file, char ch, int cnt, const char *set, const char *reset)
	fprintf(file, "%s", set);
	while (cnt--)
		putc(ch, file);
	fprintf(file, "%s", reset);
int print_stat_summary(FILE *fp, int files, int insertions, int deletions)
	int ret;
		return fprintf(fp, "%s\n", " 0 files changed");
	ret = fputs(sb.buf, fp);
	return ret;
	const char *line_prefix = "";
	line_prefix = diff_line_prefix(options);
			fprintf(options->file, "%s", line_prefix);
			show_name(options->file, prefix, name, len);
			fprintf(options->file, " %*s", number_width, "Bin");
				putc('\n', options->file);
			fprintf(options->file, " %s%"PRIuMAX"%s",
			fprintf(options->file, " -> ");
			fprintf(options->file, "%s%"PRIuMAX"%s",
			fprintf(options->file, " bytes");
			fprintf(options->file, "\n");
			fprintf(options->file, "%s", line_prefix);
			show_name(options->file, prefix, name, len);
			fprintf(options->file, " Unmerged\n");
		fprintf(options->file, "%s", line_prefix);
		show_name(options->file, prefix, name, len);
		fprintf(options->file, " %*"PRIuMAX"%s",
		show_graph(options->file, '+', add, add_c, reset);
		show_graph(options->file, '-', del, del_c, reset);
		fprintf(options->file, "\n");
			fprintf(options->file, "%s ...\n", line_prefix);
	fprintf(options->file, "%s", line_prefix);
	print_stat_summary(options->file, total_files, adds, dels);
		int deleted= data->files[i]->deleted;
	fprintf(options->file, "%s", diff_line_prefix(options));
	print_stat_summary(options->file, total_files, adds, dels);
		emit_line(data->o, set, reset, line, 1);
static void emit_binary_diff_body(FILE *file, mmfile_t *one, mmfile_t *two,
				  const char *prefix)
		fprintf(file, "%sdelta %lu\n", prefix, orig_size);
	}
	else {
		fprintf(file, "%sliteral %lu\n", prefix, two->size);
		char line[70];
		fprintf(file, "%s", prefix);
		fputs(line, file);
		fputc('\n', file);
	fprintf(file, "%s\n", prefix);
static void emit_binary_diff(FILE *file, mmfile_t *one, mmfile_t *two,
			     const char *prefix)
	fprintf(file, "%sGIT binary patch\n", prefix);
	emit_binary_diff_body(file, one, two, prefix);
	emit_binary_diff_body(file, two, one, prefix);
		show_submodule_summary(o->file, one->path ? one->path : two->path,
				line_prefix,
		show_submodule_inline_diff(o->file, one->path ? one->path : two->path,
				line_prefix,
				meta, del, add, reset, o);
			fprintf(o->file, "%s", header.buf);
		fprintf(o->file, "%s", header.buf);
					fprintf(o->file, "%s", header.buf);
			fprintf(o->file, "%s", header.buf);
			fprintf(o->file, "%sBinary files %s and %s differ\n",
				line_prefix, lbl[0], lbl[1]);
				fprintf(o->file, "%s", header.buf);
		fprintf(o->file, "%s", header.buf);
			emit_binary_diff(o->file, &mf1, &mf2, line_prefix);
			fprintf(o->file, "%sBinary files %s and %s differ\n",
				line_prefix, lbl[0], lbl[1]);
			fprintf(o->file, "%s", header.buf);
	name  = p->one->path;
	other = (strcmp(name, p->two->path) ? p->two->path : NULL);
		options->file = fopen(path, "w");
		if (!options->file)
			die_errno("Could not open '%s'", path);
static void show_file_mode_name(FILE *file, const char *newdelete, struct diff_filespec *fs)
		fprintf(file, " %s mode %06o ", newdelete, fs->mode);
		fprintf(file, " %s ", newdelete);
	write_name_quoted(fs->path, file, '\n');

static void show_mode_change(FILE *file, struct diff_filepair *p, int show_name,
		const char *line_prefix)
		fprintf(file, "%s mode change %06o => %06o%c", line_prefix, p->one->mode,
			p->two->mode, show_name ? ' ' : '\n');
			write_name_quoted(p->two->path, file, '\n');
static void show_rename_copy(FILE *file, const char *renamecopy, struct diff_filepair *p,
			const char *line_prefix)

	fprintf(file, " %s %s (%d%%)\n", renamecopy, names, similarity_index(p));
	show_mode_change(file, p, 0, line_prefix);
	FILE *file = opt->file;
	const char *line_prefix = diff_line_prefix(opt);

		fputs(line_prefix, file);
		show_file_mode_name(file, "delete", p->one);
		fputs(line_prefix, file);
		show_file_mode_name(file, "create", p->two);
		fputs(line_prefix, file);
		show_rename_copy(file, "copy", p, line_prefix);
		fputs(line_prefix, file);
		show_rename_copy(file, "rename", p, line_prefix);
			fprintf(file, "%s rewrite ", line_prefix);
			write_name_quoted(p->two->path, file, ' ');
			fprintf(file, "(%d%%)\n", similarity_index(p));
		show_mode_change(file, p, !p->score, line_prefix);
		options->file = fopen("/dev/null", "w");
		if (!options->file)
			die_errno("Could not open /dev/null");
			fprintf(options->file, "%s%c",
				diff_line_prefix(options),
				options->line_termination);
				fputs(options->stat_sep, options->file);
		for (i = 0; i < q->nr; i++) {
			struct diff_filepair *p = q->queue[i];
			if (check_pair_status(p))
				diff_flush_patch(p, options);
		}
					  df->oid.hash,
		notes_cache_put(driver->textconv_cache, df->oid.hash, *outbuf,