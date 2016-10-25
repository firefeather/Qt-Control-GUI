DROP VIEW categorylinks_view;
DROP INDEX zoom.filters_uidx;
DROP INDEX zoom.params_uname_idx;
DROP TABLE zoom.params;
DROP TABLE zoom.filters;
DROP SCHEMA zoom;

CREATE SCHEMA zoom AUTHORIZATION "admin";

CREATE TABLE zoom.filters
(
  filters_id bigserial NOT NULL,
  filters_name text NOT NULL,
  filters_user_pn text NOT NULL,
  filters_zoom_pn text NOT NULL,
  filters_filters text,
  filters_aggregators text,
  filters_complexfilter text,
  filters_selectedid bigint,
  filters_state bytea,
  filters_window integer[] NOT NULL,
  filters_masterfilter text,
  filters_masterfiltercolumn text,
  filters_readonly bool DEFAULT FALSE,
  filters_active bool DEFAULT TRUE,
  filters_savetime timestamp without time zone DEFAULT now()::timestamp without time zone,
  CONSTRAINT filters_constr UNIQUE (filters_name, filters_user_pn, filters_zoom_pn),
  CONSTRAINT filters_pkey PRIMARY KEY (filters_id)
) 
WITHOUT OIDS;
ALTER TABLE zoom.filters OWNER TO "admin";

CREATE UNIQUE INDEX filters_uidx
  ON zoom.filters
  USING btree
  (filters_user_pn, filters_zoom_pn, filters_name);


CREATE TABLE zoom.params
(
   params_id bigserial NOT NULL,
   params_uname text NOT NULL,
   params_title text,
   params_datatable text NOT NULL,
   params_active bool DEFAULT TRUE,
   params_selectionmode integer,
   params_activecolumn text,
   params_columns text,
   params_headernames text,
   params_filterzoomnames text,
   params_filterzoomcolumns text,
   params_editzoomnames text,
   params_bgcolorcolumn text,
   params_fgcolorcolumn text,
   params_rightgroups_p bigint[],
   CONSTRAINT params_uname_constr UNIQUE (params_uname),
   CONSTRAINT params_pkey PRIMARY KEY (params_id)
) 
WITHOUT OIDS;
ALTER TABLE zoom.params OWNER TO "admin";


CREATE UNIQUE INDEX params_uname_idx
  ON zoom.params
  USING btree
  (params_uname);

CREATE OR REPLACE VIEW categorylinks_view AS 
 SELECT categorylinks.cl_from, categorylinks.cl_to, categorylinks.cl_sortkey, categorylinks.cl_bigint, categorylinks.cl_bool, categorylinks.cl_date, categorylinks.cl_time, categorylinks.cl_timestamp,
        CASE
            WHEN categorylinks.cl_to ~~ 'C%'::text THEN 'navajowhite'::text
            ELSE NULL::text
        END AS cl_bgcolor,
        CASE
            WHEN categorylinks.cl_to ~~ 'B%'::text THEN 'red'::text
            ELSE NULL::text
        END AS cl_fgcolor
   FROM categorylinks;

ALTER TABLE categorylinks_view OWNER TO "admin";
