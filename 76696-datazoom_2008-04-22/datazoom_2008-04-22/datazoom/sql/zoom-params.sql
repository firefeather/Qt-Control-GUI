--
-- PostgreSQL database dump
--

SET client_encoding = 'UTF8';
SET check_function_bodies = false;
SET client_min_messages = warning;

SET search_path = zoom;

SET default_tablespace = '';

SET default_with_oids = false;




--
-- Data for Name: params; Type: TABLE DATA; Schema: zoom; Owner: admin
--

COPY params (params_id, params_uname, params_title, params_datatable, params_active, params_selectionmode, params_activecolumn, params_columns, params_headernames, params_filterzoomnames, params_filterzoomcolumns, params_editzoomnames, params_bgcolorcolumn, params_fgcolorcolumn, params_rightgroups_p) FROM stdin;
2	zoomname	Title	categorylinks_view	t	0	NULL	cl_from,cl_to,cl_sortkey,cl_bigint,cl_bool,cl_date,cl_time,cl_timestamp	From,To,Sort key,BigInt,Bool,Date,Time,TimeStamp	NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL	NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL	NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL	cl_bgcolor	cl_fgcolor	{0}
\.

--
-- PostgreSQL database dump complete
--

