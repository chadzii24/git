#include "config.h"
#include "hashmap.h"
static int diff_indent_heuristic = 1;
static int diff_color_moved_default;
	GIT_COLOR_BOLD_MAGENTA,	/* OLD_MOVED */
	GIT_COLOR_BOLD_BLUE,	/* OLD_MOVED ALTERNATIVE */
	GIT_COLOR_FAINT,	/* OLD_MOVED_DIM */
	GIT_COLOR_FAINT_ITALIC,	/* OLD_MOVED_ALTERNATIVE_DIM */
	GIT_COLOR_BOLD_CYAN,	/* NEW_MOVED */
	GIT_COLOR_BOLD_YELLOW,	/* NEW_MOVED ALTERNATIVE */
	GIT_COLOR_FAINT,	/* NEW_MOVED_DIM */
	GIT_COLOR_FAINT_ITALIC,	/* NEW_MOVED_ALTERNATIVE_DIM */
	if (!strcasecmp(var, "oldmoved"))
		return DIFF_FILE_OLD_MOVED;
	if (!strcasecmp(var, "oldmovedalternative"))
		return DIFF_FILE_OLD_MOVED_ALT;
	if (!strcasecmp(var, "oldmoveddimmed"))
		return DIFF_FILE_OLD_MOVED_DIM;
	if (!strcasecmp(var, "oldmovedalternativedimmed"))
		return DIFF_FILE_OLD_MOVED_ALT_DIM;
	if (!strcasecmp(var, "newmoved"))
		return DIFF_FILE_NEW_MOVED;
	if (!strcasecmp(var, "newmovedalternative"))
		return DIFF_FILE_NEW_MOVED_ALT;
	if (!strcasecmp(var, "newmoveddimmed"))
		return DIFF_FILE_NEW_MOVED_DIM;
	if (!strcasecmp(var, "newmovedalternativedimmed"))
		return DIFF_FILE_NEW_MOVED_ALT_DIM;
static int parse_color_moved(const char *arg)
{
	switch (git_parse_maybe_bool(arg)) {
	case 0:
		return COLOR_MOVED_NO;
	case 1:
		return COLOR_MOVED_DEFAULT;
	default:
		break;
	}

	if (!strcmp(arg, "no"))
		return COLOR_MOVED_NO;
	else if (!strcmp(arg, "plain"))
		return COLOR_MOVED_PLAIN;
	else if (!strcmp(arg, "zebra"))
		return COLOR_MOVED_ZEBRA;
	else if (!strcmp(arg, "default"))
		return COLOR_MOVED_DEFAULT;
	else if (!strcmp(arg, "dimmed_zebra"))
		return COLOR_MOVED_ZEBRA_DIM;
	else
		return error(_("color moved setting must be one of 'no', 'default', 'zebra', 'dimmed_zebra', 'plain'"));
}

	if (!strcmp(var, "diff.colormoved")) {
		int cm = parse_color_moved(value);
		if (cm < 0)
			return -1;
		diff_color_moved_default = cm;
		return 0;
	}
	if (git_diff_heuristic_config(var, value, cb) < 0)
		return -1;

enum diff_symbol {
	DIFF_SYMBOL_BINARY_DIFF_HEADER,
	DIFF_SYMBOL_BINARY_DIFF_HEADER_DELTA,
	DIFF_SYMBOL_BINARY_DIFF_HEADER_LITERAL,
	DIFF_SYMBOL_BINARY_DIFF_BODY,
	DIFF_SYMBOL_BINARY_DIFF_FOOTER,
	DIFF_SYMBOL_STATS_SUMMARY_NO_FILES,
	DIFF_SYMBOL_STATS_SUMMARY_ABBREV,
	DIFF_SYMBOL_STATS_SUMMARY_INSERTS_DELETES,
	DIFF_SYMBOL_STATS_LINE,
	DIFF_SYMBOL_WORD_DIFF,
	DIFF_SYMBOL_STAT_SEP,
	DIFF_SYMBOL_SUMMARY,
	DIFF_SYMBOL_SUBMODULE_ADD,
	DIFF_SYMBOL_SUBMODULE_DEL,
	DIFF_SYMBOL_SUBMODULE_UNTRACKED,
	DIFF_SYMBOL_SUBMODULE_MODIFIED,
	DIFF_SYMBOL_SUBMODULE_HEADER,
	DIFF_SYMBOL_SUBMODULE_ERROR,
	DIFF_SYMBOL_SUBMODULE_PIPETHROUGH,
	DIFF_SYMBOL_REWRITE_DIFF,
	DIFF_SYMBOL_BINARY_FILES,
	DIFF_SYMBOL_HEADER,
	DIFF_SYMBOL_FILEPAIR_PLUS,
	DIFF_SYMBOL_FILEPAIR_MINUS,
	DIFF_SYMBOL_WORDS_PORCELAIN,
	DIFF_SYMBOL_WORDS,
	DIFF_SYMBOL_CONTEXT,
	DIFF_SYMBOL_CONTEXT_INCOMPLETE,
	DIFF_SYMBOL_PLUS,
	DIFF_SYMBOL_MINUS,
	DIFF_SYMBOL_NO_LF_EOF,
	DIFF_SYMBOL_CONTEXT_FRAGINFO,
	DIFF_SYMBOL_CONTEXT_MARKER,
	DIFF_SYMBOL_SEPARATOR
};
/*
 * Flags for content lines:
 * 0..12 are whitespace rules
 * 13-15 are WSEH_NEW | WSEH_OLD | WSEH_CONTEXT
 * 16 is marking if the line is blank at EOF
 */
#define DIFF_SYMBOL_CONTENT_BLANK_LINE_EOF	(1<<16)
#define DIFF_SYMBOL_MOVED_LINE			(1<<17)
#define DIFF_SYMBOL_MOVED_LINE_ALT		(1<<18)
#define DIFF_SYMBOL_MOVED_LINE_UNINTERESTING	(1<<19)
#define DIFF_SYMBOL_CONTENT_WS_MASK (WSEH_NEW | WSEH_OLD | WSEH_CONTEXT | WS_RULE_MASK)

/*
 * This struct is used when we need to buffer the output of the diff output.
 *
 * NEEDSWORK: Instead of storing a copy of the line, add an offset pointer
 * into the pre/post image file. This pointer could be a union with the
 * line pointer. By storing an offset into the file instead of the literal line,
 * we can decrease the memory footprint for the buffered output. At first we
 * may want to only have indirection for the content lines, but we could also
 * enhance the state for emitting prefabricated lines, e.g. the similarity
 * score line or hunk/file headers would only need to store a number or path
 * and then the output can be constructed later on depending on state.
 */
struct emitted_diff_symbol {
	const char *line;
	int len;
	int flags;
	enum diff_symbol s;
};
#define EMITTED_DIFF_SYMBOL_INIT {NULL}

struct emitted_diff_symbols {
	struct emitted_diff_symbol *buf;
	int nr, alloc;
};
#define EMITTED_DIFF_SYMBOLS_INIT {NULL, 0, 0}

static void append_emitted_diff_symbol(struct diff_options *o,
				       struct emitted_diff_symbol *e)
	struct emitted_diff_symbol *f;

	ALLOC_GROW(o->emitted_symbols->buf,
		   o->emitted_symbols->nr + 1,
		   o->emitted_symbols->alloc);
	f = &o->emitted_symbols->buf[o->emitted_symbols->nr++];

	memcpy(f, e, sizeof(struct emitted_diff_symbol));
	f->line = e->line ? xmemdupz(e->line, e->len) : NULL;
struct moved_entry {
	struct hashmap_entry ent;
	const struct emitted_diff_symbol *es;
	struct moved_entry *next_line;
};

static int next_byte(const char **cp, const char **endp,
		     const struct diff_options *diffopt)
{
	int retval;

	if (*cp > *endp)
		return -1;

	if (DIFF_XDL_TST(diffopt, IGNORE_WHITESPACE_CHANGE)) {
		while (*cp < *endp && isspace(**cp))
			(*cp)++;
		/*
		 * After skipping a couple of whitespaces, we still have to
		 * account for one space.
		 */
		return (int)' ';
	}

	if (DIFF_XDL_TST(diffopt, IGNORE_WHITESPACE)) {
		while (*cp < *endp && isspace(**cp))
			(*cp)++;
		/* return the first non-ws character via the usual below */
	}

	retval = (unsigned char)(**cp);
	(*cp)++;
	return retval;
}

static int moved_entry_cmp(const struct diff_options *diffopt,
			   const struct moved_entry *a,
			   const struct moved_entry *b,
			   const void *keydata)
{
	const char *ap = a->es->line, *ae = a->es->line + a->es->len;
	const char *bp = b->es->line, *be = b->es->line + b->es->len;

	if (!(diffopt->xdl_opts & XDF_WHITESPACE_FLAGS))
		return a->es->len != b->es->len  || memcmp(ap, bp, a->es->len);

	if (DIFF_XDL_TST(diffopt, IGNORE_WHITESPACE_AT_EOL)) {
		while (ae > ap && isspace(*ae))
			ae--;
		while (be > bp && isspace(*be))
			be--;
	}

	while (1) {
		int ca, cb;
		ca = next_byte(&ap, &ae, diffopt);
		cb = next_byte(&bp, &be, diffopt);
		if (ca != cb)
			return 1;
		if (ca < 0)
			return 0;
	}
}

static unsigned get_string_hash(struct emitted_diff_symbol *es, struct diff_options *o)
{
	if (o->xdl_opts & XDF_WHITESPACE_FLAGS) {
		static struct strbuf sb = STRBUF_INIT;
		const char *ap = es->line, *ae = es->line + es->len;
		int c;

		strbuf_reset(&sb);
		while (ae > ap && isspace(*ae))
			ae--;
		while ((c = next_byte(&ap, &ae, o)) > 0)
			strbuf_addch(&sb, c);

		return memhash(sb.buf, sb.len);
	} else {
		return memhash(es->line, es->len);
	}
}

static struct moved_entry *prepare_entry(struct diff_options *o,
					 int line_no)
{
	struct moved_entry *ret = xmalloc(sizeof(*ret));
	struct emitted_diff_symbol *l = &o->emitted_symbols->buf[line_no];

	ret->ent.hash = get_string_hash(l, o);
	ret->es = l;
	ret->next_line = NULL;

	return ret;
}

static void add_lines_to_move_detection(struct diff_options *o,
					struct hashmap *add_lines,
					struct hashmap *del_lines)
{
	struct moved_entry *prev_line = NULL;

	int n;
	for (n = 0; n < o->emitted_symbols->nr; n++) {
		struct hashmap *hm;
		struct moved_entry *key;

		switch (o->emitted_symbols->buf[n].s) {
		case DIFF_SYMBOL_PLUS:
			hm = add_lines;
			break;
		case DIFF_SYMBOL_MINUS:
			hm = del_lines;
			break;
		default:
			prev_line = NULL;
			continue;
		}

		key = prepare_entry(o, n);
		if (prev_line && prev_line->es->s == o->emitted_symbols->buf[n].s)
			prev_line->next_line = key;

		hashmap_add(hm, key);
		prev_line = key;
	}
}

static int shrink_potential_moved_blocks(struct moved_entry **pmb,
					 int pmb_nr)
{
	int lp, rp;

	/* Shrink the set of potential block to the remaining running */
	for (lp = 0, rp = pmb_nr - 1; lp <= rp;) {
		while (lp < pmb_nr && pmb[lp])
			lp++;
		/* lp points at the first NULL now */

		while (rp > -1 && !pmb[rp])
			rp--;
		/* rp points at the last non-NULL */

		if (lp < pmb_nr && rp > -1 && lp < rp) {
			pmb[lp] = pmb[rp];
			pmb[rp] = NULL;
			rp--;
			lp++;
		}
	}

	/* Remember the number of running sets */
	return rp + 1;
}

/* Find blocks of moved code, delegate actual coloring decision to helper */
static void mark_color_as_moved(struct diff_options *o,
				struct hashmap *add_lines,
				struct hashmap *del_lines)
{
	struct moved_entry **pmb = NULL; /* potentially moved blocks */
	int pmb_nr = 0, pmb_alloc = 0;
	int n, flipped_block = 1, block_length = 0;


	for (n = 0; n < o->emitted_symbols->nr; n++) {
		struct hashmap *hm = NULL;
		struct moved_entry *key;
		struct moved_entry *match = NULL;
		struct emitted_diff_symbol *l = &o->emitted_symbols->buf[n];
		int i;

		switch (l->s) {
		case DIFF_SYMBOL_PLUS:
			hm = del_lines;
			key = prepare_entry(o, n);
			match = hashmap_get(hm, key, o);
			free(key);
			break;
		case DIFF_SYMBOL_MINUS:
			hm = add_lines;
			key = prepare_entry(o, n);
			match = hashmap_get(hm, key, o);
			free(key);
			break;
		default:
			flipped_block = 1;
		}

		if (!match) {
			if (block_length < COLOR_MOVED_MIN_BLOCK_LENGTH &&
			    o->color_moved != COLOR_MOVED_PLAIN) {
				for (i = 0; i < block_length + 1; i++) {
					l = &o->emitted_symbols->buf[n - i];
					l->flags &= ~DIFF_SYMBOL_MOVED_LINE;
				}
			}
			pmb_nr = 0;
			block_length = 0;
			continue;
		}

		l->flags |= DIFF_SYMBOL_MOVED_LINE;
		block_length++;

		if (o->color_moved == COLOR_MOVED_PLAIN)
			continue;

		/* Check any potential block runs, advance each or nullify */
		for (i = 0; i < pmb_nr; i++) {
			struct moved_entry *p = pmb[i];
			struct moved_entry *pnext = (p && p->next_line) ?
					p->next_line : NULL;
			if (pnext && !hm->cmpfn(o, pnext, match, NULL)) {
				pmb[i] = p->next_line;
			} else {
				pmb[i] = NULL;
			}
		}

		pmb_nr = shrink_potential_moved_blocks(pmb, pmb_nr);

		if (pmb_nr == 0) {
			/*
			 * The current line is the start of a new block.
			 * Setup the set of potential blocks.
			 */
			for (; match; match = hashmap_get_next(hm, match)) {
				ALLOC_GROW(pmb, pmb_nr + 1, pmb_alloc);
				pmb[pmb_nr++] = match;
			}

			flipped_block = (flipped_block + 1) % 2;
		}

		if (flipped_block)
			l->flags |= DIFF_SYMBOL_MOVED_LINE_ALT;
	}

	free(pmb);
}

#define DIFF_SYMBOL_MOVED_LINE_ZEBRA_MASK \
  (DIFF_SYMBOL_MOVED_LINE | DIFF_SYMBOL_MOVED_LINE_ALT)
static void dim_moved_lines(struct diff_options *o)
{
	int n;
	for (n = 0; n < o->emitted_symbols->nr; n++) {
		struct emitted_diff_symbol *prev = (n != 0) ?
				&o->emitted_symbols->buf[n - 1] : NULL;
		struct emitted_diff_symbol *l = &o->emitted_symbols->buf[n];
		struct emitted_diff_symbol *next =
				(n < o->emitted_symbols->nr - 1) ?
				&o->emitted_symbols->buf[n + 1] : NULL;

		/* Not a plus or minus line? */
		if (l->s != DIFF_SYMBOL_PLUS && l->s != DIFF_SYMBOL_MINUS)
			continue;

		/* Not a moved line? */
		if (!(l->flags & DIFF_SYMBOL_MOVED_LINE))
			continue;

		/*
		 * If prev or next are not a plus or minus line,
		 * pretend they don't exist
		 */
		if (prev && prev->s != DIFF_SYMBOL_PLUS &&
			    prev->s != DIFF_SYMBOL_MINUS)
			prev = NULL;
		if (next && next->s != DIFF_SYMBOL_PLUS &&
			    next->s != DIFF_SYMBOL_MINUS)
			next = NULL;

		/* Inside a block? */
		if ((prev &&
		    (prev->flags & DIFF_SYMBOL_MOVED_LINE_ZEBRA_MASK) ==
		    (l->flags & DIFF_SYMBOL_MOVED_LINE_ZEBRA_MASK)) &&
		    (next &&
		    (next->flags & DIFF_SYMBOL_MOVED_LINE_ZEBRA_MASK) ==
		    (l->flags & DIFF_SYMBOL_MOVED_LINE_ZEBRA_MASK))) {
			l->flags |= DIFF_SYMBOL_MOVED_LINE_UNINTERESTING;
			continue;
		}

		/* Check if we are at an interesting bound: */
		if (prev && (prev->flags & DIFF_SYMBOL_MOVED_LINE) &&
		    (prev->flags & DIFF_SYMBOL_MOVED_LINE_ALT) !=
		       (l->flags & DIFF_SYMBOL_MOVED_LINE_ALT))
			continue;
		if (next && (next->flags & DIFF_SYMBOL_MOVED_LINE) &&
		    (next->flags & DIFF_SYMBOL_MOVED_LINE_ALT) !=
		       (l->flags & DIFF_SYMBOL_MOVED_LINE_ALT))
			continue;

		/*
		 * The boundary to prev and next are not interesting,
		 * so this line is not interesting as a whole
		 */
		l->flags |= DIFF_SYMBOL_MOVED_LINE_UNINTERESTING;
	}
}

static void emit_line_ws_markup(struct diff_options *o,
				const char *set, const char *reset,
				const char *line, int len, char sign,
				unsigned ws_rule, int blank_at_eof)
	if (o->ws_error_highlight & ws_rule) {
		ws = diff_get_color_opt(o, DIFF_WHITESPACE);
		emit_line_0(o, set, reset, sign, line, len);
	else if (blank_at_eof)
		emit_line_0(o, ws, reset, sign, line, len);
		emit_line_0(o, set, reset, sign, "", 0);
		ws_check_emit(line, len, ws_rule,
			      o->file, set, reset, ws);
static void emit_diff_symbol_from_struct(struct diff_options *o,
					 struct emitted_diff_symbol *eds)
{
	static const char *nneof = " No newline at end of file\n";
	const char *context, *reset, *set, *meta, *fraginfo;
	struct strbuf sb = STRBUF_INIT;

	enum diff_symbol s = eds->s;
	const char *line = eds->line;
	int len = eds->len;
	unsigned flags = eds->flags;

	switch (s) {
	case DIFF_SYMBOL_NO_LF_EOF:
		context = diff_get_color_opt(o, DIFF_CONTEXT);
		reset = diff_get_color_opt(o, DIFF_RESET);
		putc('\n', o->file);
		emit_line_0(o, context, reset, '\\',
			    nneof, strlen(nneof));
		break;
	case DIFF_SYMBOL_SUBMODULE_HEADER:
	case DIFF_SYMBOL_SUBMODULE_ERROR:
	case DIFF_SYMBOL_SUBMODULE_PIPETHROUGH:
	case DIFF_SYMBOL_STATS_SUMMARY_INSERTS_DELETES:
	case DIFF_SYMBOL_SUMMARY:
	case DIFF_SYMBOL_STATS_LINE:
	case DIFF_SYMBOL_BINARY_DIFF_BODY:
	case DIFF_SYMBOL_CONTEXT_FRAGINFO:
		emit_line(o, "", "", line, len);
		break;
	case DIFF_SYMBOL_CONTEXT_INCOMPLETE:
	case DIFF_SYMBOL_CONTEXT_MARKER:
		context = diff_get_color_opt(o, DIFF_CONTEXT);
		reset = diff_get_color_opt(o, DIFF_RESET);
		emit_line(o, context, reset, line, len);
		break;
	case DIFF_SYMBOL_SEPARATOR:
		fprintf(o->file, "%s%c",
			diff_line_prefix(o),
			o->line_termination);
		break;
	case DIFF_SYMBOL_CONTEXT:
		set = diff_get_color_opt(o, DIFF_CONTEXT);
		reset = diff_get_color_opt(o, DIFF_RESET);
		emit_line_ws_markup(o, set, reset, line, len, ' ',
				    flags & (DIFF_SYMBOL_CONTENT_WS_MASK), 0);
		break;
	case DIFF_SYMBOL_PLUS:
		switch (flags & (DIFF_SYMBOL_MOVED_LINE |
				 DIFF_SYMBOL_MOVED_LINE_ALT |
				 DIFF_SYMBOL_MOVED_LINE_UNINTERESTING)) {
		case DIFF_SYMBOL_MOVED_LINE |
		     DIFF_SYMBOL_MOVED_LINE_ALT |
		     DIFF_SYMBOL_MOVED_LINE_UNINTERESTING:
			set = diff_get_color_opt(o, DIFF_FILE_NEW_MOVED_ALT_DIM);
			break;
		case DIFF_SYMBOL_MOVED_LINE |
		     DIFF_SYMBOL_MOVED_LINE_ALT:
			set = diff_get_color_opt(o, DIFF_FILE_NEW_MOVED_ALT);
			break;
		case DIFF_SYMBOL_MOVED_LINE |
		     DIFF_SYMBOL_MOVED_LINE_UNINTERESTING:
			set = diff_get_color_opt(o, DIFF_FILE_NEW_MOVED_DIM);
			break;
		case DIFF_SYMBOL_MOVED_LINE:
			set = diff_get_color_opt(o, DIFF_FILE_NEW_MOVED);
			break;
		default:
			set = diff_get_color_opt(o, DIFF_FILE_NEW);
		}
		reset = diff_get_color_opt(o, DIFF_RESET);
		emit_line_ws_markup(o, set, reset, line, len, '+',
				    flags & DIFF_SYMBOL_CONTENT_WS_MASK,
				    flags & DIFF_SYMBOL_CONTENT_BLANK_LINE_EOF);
		break;
	case DIFF_SYMBOL_MINUS:
		switch (flags & (DIFF_SYMBOL_MOVED_LINE |
				 DIFF_SYMBOL_MOVED_LINE_ALT |
				 DIFF_SYMBOL_MOVED_LINE_UNINTERESTING)) {
		case DIFF_SYMBOL_MOVED_LINE |
		     DIFF_SYMBOL_MOVED_LINE_ALT |
		     DIFF_SYMBOL_MOVED_LINE_UNINTERESTING:
			set = diff_get_color_opt(o, DIFF_FILE_OLD_MOVED_ALT_DIM);
			break;
		case DIFF_SYMBOL_MOVED_LINE |
		     DIFF_SYMBOL_MOVED_LINE_ALT:
			set = diff_get_color_opt(o, DIFF_FILE_OLD_MOVED_ALT);
			break;
		case DIFF_SYMBOL_MOVED_LINE |
		     DIFF_SYMBOL_MOVED_LINE_UNINTERESTING:
			set = diff_get_color_opt(o, DIFF_FILE_OLD_MOVED_DIM);
			break;
		case DIFF_SYMBOL_MOVED_LINE:
			set = diff_get_color_opt(o, DIFF_FILE_OLD_MOVED);
			break;
		default:
			set = diff_get_color_opt(o, DIFF_FILE_OLD);
		}
		reset = diff_get_color_opt(o, DIFF_RESET);
		emit_line_ws_markup(o, set, reset, line, len, '-',
				    flags & DIFF_SYMBOL_CONTENT_WS_MASK, 0);
		break;
	case DIFF_SYMBOL_WORDS_PORCELAIN:
		context = diff_get_color_opt(o, DIFF_CONTEXT);
		reset = diff_get_color_opt(o, DIFF_RESET);
		emit_line(o, context, reset, line, len);
		fputs("~\n", o->file);
		break;
	case DIFF_SYMBOL_WORDS:
		context = diff_get_color_opt(o, DIFF_CONTEXT);
		reset = diff_get_color_opt(o, DIFF_RESET);
		/*
		 * Skip the prefix character, if any.  With
		 * diff_suppress_blank_empty, there may be
		 * none.
		 */
		if (line[0] != '\n') {
			line++;
			len--;
		}
		emit_line(o, context, reset, line, len);
		break;
	case DIFF_SYMBOL_FILEPAIR_PLUS:
		meta = diff_get_color_opt(o, DIFF_METAINFO);
		reset = diff_get_color_opt(o, DIFF_RESET);
		fprintf(o->file, "%s%s+++ %s%s%s\n", diff_line_prefix(o), meta,
			line, reset,
			strchr(line, ' ') ? "\t" : "");
		break;
	case DIFF_SYMBOL_FILEPAIR_MINUS:
		meta = diff_get_color_opt(o, DIFF_METAINFO);
		reset = diff_get_color_opt(o, DIFF_RESET);
		fprintf(o->file, "%s%s--- %s%s%s\n", diff_line_prefix(o), meta,
			line, reset,
			strchr(line, ' ') ? "\t" : "");
		break;
	case DIFF_SYMBOL_BINARY_FILES:
	case DIFF_SYMBOL_HEADER:
		fprintf(o->file, "%s", line);
		break;
	case DIFF_SYMBOL_BINARY_DIFF_HEADER:
		fprintf(o->file, "%sGIT binary patch\n", diff_line_prefix(o));
		break;
	case DIFF_SYMBOL_BINARY_DIFF_HEADER_DELTA:
		fprintf(o->file, "%sdelta %s\n", diff_line_prefix(o), line);
		break;
	case DIFF_SYMBOL_BINARY_DIFF_HEADER_LITERAL:
		fprintf(o->file, "%sliteral %s\n", diff_line_prefix(o), line);
		break;
	case DIFF_SYMBOL_BINARY_DIFF_FOOTER:
		fputs(diff_line_prefix(o), o->file);
		fputc('\n', o->file);
		break;
	case DIFF_SYMBOL_REWRITE_DIFF:
		fraginfo = diff_get_color(o->use_color, DIFF_FRAGINFO);
		reset = diff_get_color_opt(o, DIFF_RESET);
		emit_line(o, fraginfo, reset, line, len);
		break;
	case DIFF_SYMBOL_SUBMODULE_ADD:
		set = diff_get_color_opt(o, DIFF_FILE_NEW);
		reset = diff_get_color_opt(o, DIFF_RESET);
		emit_line(o, set, reset, line, len);
		break;
	case DIFF_SYMBOL_SUBMODULE_DEL:
		set = diff_get_color_opt(o, DIFF_FILE_OLD);
		reset = diff_get_color_opt(o, DIFF_RESET);
		emit_line(o, set, reset, line, len);
		break;
	case DIFF_SYMBOL_SUBMODULE_UNTRACKED:
		fprintf(o->file, "%sSubmodule %s contains untracked content\n",
			diff_line_prefix(o), line);
		break;
	case DIFF_SYMBOL_SUBMODULE_MODIFIED:
		fprintf(o->file, "%sSubmodule %s contains modified content\n",
			diff_line_prefix(o), line);
		break;
	case DIFF_SYMBOL_STATS_SUMMARY_NO_FILES:
		emit_line(o, "", "", " 0 files changed\n",
			  strlen(" 0 files changed\n"));
		break;
	case DIFF_SYMBOL_STATS_SUMMARY_ABBREV:
		emit_line(o, "", "", " ...\n", strlen(" ...\n"));
		break;
	case DIFF_SYMBOL_WORD_DIFF:
		fprintf(o->file, "%.*s", len, line);
		break;
	case DIFF_SYMBOL_STAT_SEP:
		fputs(o->stat_sep, o->file);
		break;
	default:
		die("BUG: unknown diff symbol");
	}
	strbuf_release(&sb);
}

static void emit_diff_symbol(struct diff_options *o, enum diff_symbol s,
			     const char *line, int len, unsigned flags)
{
	struct emitted_diff_symbol e = {line, len, flags, s};

	if (o->emitted_symbols)
		append_emitted_diff_symbol(o, &e);
	else
		emit_diff_symbol_from_struct(o, &e);
}

void diff_emit_submodule_del(struct diff_options *o, const char *line)
{
	emit_diff_symbol(o, DIFF_SYMBOL_SUBMODULE_DEL, line, strlen(line), 0);
}

void diff_emit_submodule_add(struct diff_options *o, const char *line)
{
	emit_diff_symbol(o, DIFF_SYMBOL_SUBMODULE_ADD, line, strlen(line), 0);
}

void diff_emit_submodule_untracked(struct diff_options *o, const char *path)
{
	emit_diff_symbol(o, DIFF_SYMBOL_SUBMODULE_UNTRACKED,
			 path, strlen(path), 0);
}

void diff_emit_submodule_modified(struct diff_options *o, const char *path)
{
	emit_diff_symbol(o, DIFF_SYMBOL_SUBMODULE_MODIFIED,
			 path, strlen(path), 0);
}

void diff_emit_submodule_header(struct diff_options *o, const char *header)
{
	emit_diff_symbol(o, DIFF_SYMBOL_SUBMODULE_HEADER,
			 header, strlen(header), 0);
}

void diff_emit_submodule_error(struct diff_options *o, const char *err)
{
	emit_diff_symbol(o, DIFF_SYMBOL_SUBMODULE_ERROR, err, strlen(err), 0);
}

void diff_emit_submodule_pipethrough(struct diff_options *o,
				     const char *line, int len)
{
	emit_diff_symbol(o, DIFF_SYMBOL_SUBMODULE_PIPETHROUGH, line, len, 0);
}

static int new_blank_line_at_eof(struct emit_callback *ecbdata, const char *line, int len)
{
	if (!((ecbdata->ws_rule & WS_BLANK_AT_EOF) &&
	      ecbdata->blank_at_eof_in_preimage &&
	      ecbdata->blank_at_eof_in_postimage &&
	      ecbdata->blank_at_eof_in_preimage <= ecbdata->lno_in_preimage &&
	      ecbdata->blank_at_eof_in_postimage <= ecbdata->lno_in_postimage))
		return 0;
	return ws_blank_line(line, len, ecbdata->ws_rule);
}

	unsigned flags = WSEH_NEW | ecbdata->ws_rule;
	if (new_blank_line_at_eof(ecbdata, line, len))
		flags |= DIFF_SYMBOL_CONTENT_BLANK_LINE_EOF;

	emit_diff_symbol(ecbdata->opt, DIFF_SYMBOL_PLUS, line, len, flags);
	unsigned flags = WSEH_OLD | ecbdata->ws_rule;
	emit_diff_symbol(ecbdata->opt, DIFF_SYMBOL_MINUS, line, len, flags);
	unsigned flags = WSEH_CONTEXT | ecbdata->ws_rule;
	emit_diff_symbol(ecbdata->opt, DIFF_SYMBOL_CONTEXT, line, len, flags);
		emit_diff_symbol(ecbdata->opt,
				 DIFF_SYMBOL_CONTEXT_MARKER, line, len, 0);
	strbuf_complete_line(&msgbuf);
	emit_diff_symbol(ecbdata->opt,
			 DIFF_SYMBOL_CONTEXT_FRAGINFO, msgbuf.buf, msgbuf.len, 0);
static void add_line_count(struct strbuf *out, int count)
		strbuf_addstr(out, "0,0");
		strbuf_addstr(out, "1");
		strbuf_addf(out, "1,%d", count);
	if (!endp)
		emit_diff_symbol(ecb->opt, DIFF_SYMBOL_NO_LF_EOF, NULL, 0, 0);
	struct strbuf out = STRBUF_INIT;

	emit_diff_symbol(o, DIFF_SYMBOL_FILEPAIR_MINUS,
			 a_name.buf, a_name.len, 0);
	emit_diff_symbol(o, DIFF_SYMBOL_FILEPAIR_PLUS,
			 b_name.buf, b_name.len, 0);

	strbuf_addstr(&out, "@@ -");
		add_line_count(&out, lc_a);
		strbuf_addstr(&out, "?,?");
	strbuf_addstr(&out, " +");
	add_line_count(&out, lc_b);
	strbuf_addstr(&out, " @@\n");
	emit_diff_symbol(o, DIFF_SYMBOL_REWRITE_DIFF, out.buf, out.len, 0);
	strbuf_release(&out);

static int fn_out_diff_words_write_helper(struct diff_options *o,
					  size_t count, const char *buf)
	struct strbuf sb = STRBUF_INIT;
			strbuf_addstr(&sb, diff_line_prefix(o));

			const char *reset = st_el->color && *st_el->color ?
					    GIT_COLOR_RESET : NULL;
			if (st_el->color && *st_el->color)
				strbuf_addstr(&sb, st_el->color);
			strbuf_addstr(&sb, st_el->prefix);
			strbuf_add(&sb, buf, p ? p - buf : count);
			strbuf_addstr(&sb, st_el->suffix);
			if (reset)
				strbuf_addstr(&sb, reset);
			goto out;

		strbuf_addstr(&sb, newline);
		if (count) {
			emit_diff_symbol(o, DIFF_SYMBOL_WORD_DIFF,
					 sb.buf, sb.len, 0);
			strbuf_reset(&sb);
		}

out:
	if (sb.len)
		emit_diff_symbol(o, DIFF_SYMBOL_WORD_DIFF,
				 sb.buf, sb.len, 0);
	strbuf_release(&sb);
		fn_out_diff_words_write_helper(diff_words->opt,
				diff_words->current_plus);
		fn_out_diff_words_write_helper(diff_words->opt,
				minus_end - minus_begin, minus_begin);
		fn_out_diff_words_write_helper(diff_words->opt,
				plus_end - plus_begin, plus_begin);
		emit_diff_symbol(diff_words->opt, DIFF_SYMBOL_WORD_DIFF,
				 line_prefix, strlen(line_prefix), 0);
		fn_out_diff_words_write_helper(diff_words->opt,
			diff_words->minus.text.ptr);
			emit_diff_symbol(diff_words->opt, DIFF_SYMBOL_WORD_DIFF,
					 line_prefix, strlen(line_prefix), 0);
		fn_out_diff_words_write_helper(diff_words->opt,
			- diff_words->current_plus, diff_words->current_plus);
	struct diff_options *wo = ecbdata->diff_words->opt;


	if (wo->emitted_symbols) {
		struct diff_options *o = ecbdata->opt;
		struct emitted_diff_symbols *wol = wo->emitted_symbols;
		int i;

		/*
		 * NEEDSWORK:
		 * Instead of appending each, concat all words to a line?
		 */
		for (i = 0; i < wol->nr; i++)
			append_emitted_diff_symbol(o, &wol->buf[i]);

		for (i = 0; i < wol->nr; i++)
			free((void *)wol->buf[i].line);

		wol->nr = 0;
	}

	if (orig_opts->emitted_symbols)
		o->emitted_symbols =
			xcalloc(1, sizeof(struct emitted_diff_symbols));

		free (ecbdata->diff_words->opt->emitted_symbols);
		FREE_AND_NULL(ecbdata->diff_words);
		emit_diff_symbol(o, DIFF_SYMBOL_HEADER,
				 ecbdata->header->buf, ecbdata->header->len, 0);
		emit_diff_symbol(o, DIFF_SYMBOL_FILEPAIR_MINUS,
				 ecbdata->label_path[0],
				 strlen(ecbdata->label_path[0]), 0);
		emit_diff_symbol(o, DIFF_SYMBOL_FILEPAIR_PLUS,
				 ecbdata->label_path[1],
				 strlen(ecbdata->label_path[1]), 0);
		enum diff_symbol s =
			ecbdata->diff_words->type == DIFF_WORDS_PORCELAIN ?
			DIFF_SYMBOL_WORDS_PORCELAIN : DIFF_SYMBOL_WORDS;
		emit_diff_symbol(o, s, line, len, 0);
		emit_diff_symbol(o, DIFF_SYMBOL_CONTEXT_INCOMPLETE,
				 line, len, 0);
static void show_graph(struct strbuf *out, char ch, int cnt,
		       const char *set, const char *reset)
	strbuf_addstr(out, set);
	strbuf_addchars(out, ch, cnt);
	strbuf_addstr(out, reset);
static void print_stat_summary_inserts_deletes(struct diff_options *options,
		int files, int insertions, int deletions)
		emit_diff_symbol(options, DIFF_SYMBOL_STATS_SUMMARY_NO_FILES,
				 NULL, 0, 0);
		return;
	emit_diff_symbol(options, DIFF_SYMBOL_STATS_SUMMARY_INSERTS_DELETES,
			 sb.buf, sb.len, 0);
}

void print_stat_summary(FILE *fp, int files,
			int insertions, int deletions)
{
	struct diff_options o;
	memset(&o, 0, sizeof(o));
	o.file = fp;

	print_stat_summary_inserts_deletes(&o, files, insertions, deletions);
	const char *line_prefix = diff_line_prefix(options);
	struct strbuf out = STRBUF_INIT;
			strbuf_addf(&out, " %s%-*s |", prefix, len, name);
			strbuf_addf(&out, " %*s", number_width, "Bin");
				strbuf_addch(&out, '\n');
				emit_diff_symbol(options, DIFF_SYMBOL_STATS_LINE,
						 out.buf, out.len, 0);
				strbuf_reset(&out);
			strbuf_addf(&out, " %s%"PRIuMAX"%s",
			strbuf_addstr(&out, " -> ");
			strbuf_addf(&out, "%s%"PRIuMAX"%s",
			strbuf_addstr(&out, " bytes\n");
			emit_diff_symbol(options, DIFF_SYMBOL_STATS_LINE,
					 out.buf, out.len, 0);
			strbuf_reset(&out);
			strbuf_addf(&out, " %s%-*s |", prefix, len, name);
			strbuf_addstr(&out, " Unmerged\n");
			emit_diff_symbol(options, DIFF_SYMBOL_STATS_LINE,
					 out.buf, out.len, 0);
			strbuf_reset(&out);
		strbuf_addf(&out, " %s%-*s |", prefix, len, name);
		strbuf_addf(&out, " %*"PRIuMAX"%s",
		show_graph(&out, '+', add, add_c, reset);
		show_graph(&out, '-', del, del_c, reset);
		strbuf_addch(&out, '\n');
		emit_diff_symbol(options, DIFF_SYMBOL_STATS_LINE,
				 out.buf, out.len, 0);
		strbuf_reset(&out);
			emit_diff_symbol(options,
					 DIFF_SYMBOL_STATS_SUMMARY_ABBREV,
					 NULL, 0, 0);

	print_stat_summary_inserts_deletes(options, total_files, adds, dels);
		int deleted = data->files[i]->deleted;
	print_stat_summary_inserts_deletes(options, total_files, adds, dels);
			damage = DIV_ROUND_UP(damage, 64);
static void emit_binary_diff_body(struct diff_options *o,
				  mmfile_t *one, mmfile_t *two)
		char *s = xstrfmt("%lu", orig_size);
		emit_diff_symbol(o, DIFF_SYMBOL_BINARY_DIFF_HEADER_DELTA,
				 s, strlen(s), 0);
		free(s);
	} else {
		char *s = xstrfmt("%lu", two->size);
		emit_diff_symbol(o, DIFF_SYMBOL_BINARY_DIFF_HEADER_LITERAL,
				 s, strlen(s), 0);
		free(s);
		int len;
		char line[71];

		len = strlen(line);
		line[len++] = '\n';
		line[len] = '\0';

		emit_diff_symbol(o, DIFF_SYMBOL_BINARY_DIFF_BODY,
				 line, len, 0);
	emit_diff_symbol(o, DIFF_SYMBOL_BINARY_DIFF_FOOTER, NULL, 0, 0);
static void emit_binary_diff(struct diff_options *o,
			     mmfile_t *one, mmfile_t *two)
	emit_diff_symbol(o, DIFF_SYMBOL_BINARY_DIFF_HEADER, NULL, 0, 0);
	emit_binary_diff_body(o, one, two);
	emit_binary_diff_body(o, two, one);
		show_submodule_summary(o, one->path ? one->path : two->path,
				two->dirty_submodule);
		show_submodule_inline_diff(o, one->path ? one->path : two->path,
				two->dirty_submodule);
			emit_diff_symbol(o, DIFF_SYMBOL_HEADER,
					 header.buf, header.len, 0);
		emit_diff_symbol(o, DIFF_SYMBOL_HEADER, header.buf,
				 header.len, 0);
		struct strbuf sb = STRBUF_INIT;
					emit_diff_symbol(o, DIFF_SYMBOL_HEADER,
							 header.buf, header.len,
							 0);
			emit_diff_symbol(o, DIFF_SYMBOL_HEADER,
					 header.buf, header.len, 0);
			strbuf_addf(&sb, "%sBinary files %s and %s differ\n",
				    diff_line_prefix(o), lbl[0], lbl[1]);
			emit_diff_symbol(o, DIFF_SYMBOL_BINARY_FILES,
					 sb.buf, sb.len, 0);
			strbuf_release(&sb);
				emit_diff_symbol(o, DIFF_SYMBOL_HEADER,
						 header.buf, header.len, 0);
		emit_diff_symbol(o, DIFF_SYMBOL_HEADER, header.buf, header.len, 0);
			emit_binary_diff(o, &mf1, &mf2);
		else {
			strbuf_addf(&sb, "%sBinary files %s and %s differ\n",
				    diff_line_prefix(o), lbl[0], lbl[1]);
			emit_diff_symbol(o, DIFF_SYMBOL_BINARY_FILES,
					 sb.buf, sb.len, 0);
			strbuf_release(&sb);
		}
			emit_diff_symbol(o, DIFF_SYMBOL_HEADER,
					 header.buf, header.len, 0);
void fill_filespec(struct diff_filespec *spec, const struct object_id *oid,
		   int oid_valid, unsigned short mode)
		oidcpy(&spec->oid, oid);
		spec->oid_valid = oid_valid;
static int reuse_worktree_file(const char *name, const struct object_id *oid, int want_file)
	if (!FAST_WORKING_DIRECTORY && !want_file && has_sha1_pack(oid->hash))
	if (!want_file && would_convert_to_git(&the_index, name))
	if (oidcmp(oid, &ce->oid) || !S_ISREG(ce->ce_mode))
	    reuse_worktree_file(s->path, &s->oid, 0)) {
		if (size_only && !would_convert_to_git(&the_index, s->path))
		if (convert_to_git(&the_index, s->path, s->data, s->size, &buf, crlf_warn)) {
	FREE_AND_NULL(s->cnt_data);
	     reuse_worktree_file(name, &one->oid, 1))) {
				oid_to_hex_r(temp->hex, &null_oid);
			 * !(one->oid_valid), as long as
static void diff_fill_oid_info(struct diff_filespec *one)
	name  = one->path;
	other = (strcmp(name, two->path) ? two->path : NULL);
	diff_fill_oid_info(one);
	diff_fill_oid_info(two);
	diff_fill_oid_info(p->one);
	diff_fill_oid_info(p->two);
	diff_fill_oid_info(p->one);
	diff_fill_oid_info(p->two);

	options->color_moved = diff_color_moved_default;

	if (!options->use_color || external_diff())
		options->color_moved = 0;
	else if (!strcmp(arg, "--color-moved")) {
		if (diff_color_moved_default)
			options->color_moved = diff_color_moved_default;
		if (options->color_moved == COLOR_MOVED_NO)
			options->color_moved = COLOR_MOVED_DEFAULT;
	} else if (!strcmp(arg, "--no-color-moved"))
		options->color_moved = COLOR_MOVED_NO;
	else if (skip_prefix(arg, "--color-moved=", &arg)) {
		int cm = parse_color_moved(arg);
		if (cm < 0)
			die("bad --color-moved argument: %s", arg);
		options->color_moved = cm;
	} else if (!strcmp(arg, "--color-words")) {
		options->file = xfopen(path, "w");
static void show_file_mode_name(struct diff_options *opt, const char *newdelete, struct diff_filespec *fs)
	struct strbuf sb = STRBUF_INIT;
		strbuf_addf(&sb, " %s mode %06o ", newdelete, fs->mode);
		strbuf_addf(&sb, " %s ", newdelete);
	quote_c_style(fs->path, &sb, NULL, 0);
	strbuf_addch(&sb, '\n');
	emit_diff_symbol(opt, DIFF_SYMBOL_SUMMARY,
			 sb.buf, sb.len, 0);
	strbuf_release(&sb);
}
static void show_mode_change(struct diff_options *opt, struct diff_filepair *p,
		int show_name)
		struct strbuf sb = STRBUF_INIT;
		strbuf_addf(&sb, " mode change %06o => %06o",
			    p->one->mode, p->two->mode);
			strbuf_addch(&sb, ' ');
			quote_c_style(p->two->path, &sb, NULL, 0);
		emit_diff_symbol(opt, DIFF_SYMBOL_SUMMARY,
				 sb.buf, sb.len, 0);
		strbuf_release(&sb);
static void show_rename_copy(struct diff_options *opt, const char *renamecopy,
		struct diff_filepair *p)
	struct strbuf sb = STRBUF_INIT;
	strbuf_addf(&sb, " %s %s (%d%%)\n",
			renamecopy, names, similarity_index(p));
	emit_diff_symbol(opt, DIFF_SYMBOL_SUMMARY,
				 sb.buf, sb.len, 0);
	show_mode_change(opt, p, 0);
		show_file_mode_name(opt, "delete", p->one);
		show_file_mode_name(opt, "create", p->two);
		show_rename_copy(opt, "copy", p);
		show_rename_copy(opt, "rename", p);
			struct strbuf sb = STRBUF_INIT;
			strbuf_addstr(&sb, " rewrite ");
			quote_c_style(p->two->path, &sb, NULL, 0);
			strbuf_addf(&sb, " (%d%%)\n", similarity_index(p));
			emit_diff_symbol(opt, DIFF_SYMBOL_SUMMARY,
					 sb.buf, sb.len, 0);
		show_mode_change(opt, p, !p->score);
static int diff_get_patch_id(struct diff_options *options, struct object_id *oid, int diff_header_only)
		diff_fill_oid_info(p->one);
		diff_fill_oid_info(p->two);
					GIT_SHA1_HEXSZ);
					GIT_SHA1_HEXSZ);
	git_SHA1_Final(oid->hash, &ctx);
int diff_flush_patch_id(struct diff_options *options, struct object_id *oid, int diff_header_only)
	int result = diff_get_patch_id(options, oid, diff_header_only);
static void diff_flush_patch_all_file_pairs(struct diff_options *o)
{
	int i;
	static struct emitted_diff_symbols esm = EMITTED_DIFF_SYMBOLS_INIT;
	struct diff_queue_struct *q = &diff_queued_diff;

	if (WSEH_NEW & WS_RULE_MASK)
		die("BUG: WS rules bit mask overlaps with diff symbol flags");

	if (o->color_moved)
		o->emitted_symbols = &esm;

	for (i = 0; i < q->nr; i++) {
		struct diff_filepair *p = q->queue[i];
		if (check_pair_status(p))
			diff_flush_patch(p, o);
	}

	if (o->emitted_symbols) {
		if (o->color_moved) {
			struct hashmap add_lines, del_lines;

			hashmap_init(&del_lines,
				     (hashmap_cmp_fn)moved_entry_cmp, o, 0);
			hashmap_init(&add_lines,
				     (hashmap_cmp_fn)moved_entry_cmp, o, 0);

			add_lines_to_move_detection(o, &add_lines, &del_lines);
			mark_color_as_moved(o, &add_lines, &del_lines);
			if (o->color_moved == COLOR_MOVED_ZEBRA_DIM)
				dim_moved_lines(o);

			hashmap_free(&add_lines, 0);
			hashmap_free(&del_lines, 0);
		}

		for (i = 0; i < esm.nr; i++)
			emit_diff_symbol_from_struct(o, &esm.buf[i]);

		for (i = 0; i < esm.nr; i++)
			free((void *)esm.buf[i].line);
	}
	esm.nr = 0;
}

		options->file = xfopen("/dev/null", "w");
		options->color_moved = 0;
			emit_diff_symbol(options, DIFF_SYMBOL_SEPARATOR, NULL, 0, 0);
			if (options->stat_sep)
				emit_diff_symbol(options, DIFF_SYMBOL_STAT_SEP,
						 NULL, 0, 0);
		diff_flush_patch_all_file_pairs(options);
		    const struct object_id *oid,
		    int oid_valid,
		fill_filespec(one, oid, oid_valid, mode);
		fill_filespec(two, oid, oid_valid, mode);
		 const struct object_id *old_oid,
		 const struct object_id *new_oid,
		 int old_oid_valid, int new_oid_valid,
		SWAP(old_oid, new_oid);
		SWAP(old_oid_valid, new_oid_valid);
	fill_filespec(one, old_oid, old_oid_valid, old_mode);
	fill_filespec(two, new_oid, new_oid_valid, new_mode);
					  &df->oid,
		notes_cache_put(driver->textconv_cache, &df->oid, *outbuf,
int textconv_object(const char *path,
		    unsigned mode,
		    const struct object_id *oid,
		    int oid_valid,
		    char **buf,
		    unsigned long *buf_size)
{
	struct diff_filespec *df;
	struct userdiff_driver *textconv;

	df = alloc_filespec(path);
	fill_filespec(df, oid, oid_valid, mode);
	textconv = get_textconv(df);
	if (!textconv) {
		free_filespec(df);
		return 0;
	}

	*buf_size = fill_textconv(textconv, df, buf);
	free_filespec(df);
	return 1;
}
