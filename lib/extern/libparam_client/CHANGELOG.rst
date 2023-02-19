Parameter System (libparam_client)
==================================

**4.8.1 (2020-03-19)**

- Feature: Added gs_rparam_get_array() for getting an array of parameters.
- Breaking: Python/rparam, removed automatic load of tables on module import/load.
- Feature: Python/rparam, added rparam_load_tables_from_dir().

**4.7.3 (2019-12-03)**

- Improvement: param_gen, improved formatting of 'validation' - 'bits' and 'discrete'.
- Improvement: param_gen: added support for new line (\n) in parameter description.

**4.7.2 (2019-10-16)**

- Improvement: Clear rparam query and cached table data on rparam init/download.

**4.7.1 (2019-05-16)**

- Improvement: Updated libutil dependency and fixed warning due to const improvement in libutil.

**4.6.1 (2019-04-09)**

- Bug: fixed issue with rparam download (failed to save table if working folder > 100 characters).
- Improvement: Updated dependencies.

**4.5.4 (2019-02-26)**

- Improvement: Adjusted table columnspace in auto-generated rsti files.

**4.5.3 (2019-01-29)**

- Feature: Added rparam_set_data/rparam_get_data to bindings.

**4.5.2 (2018-12-19)**

- Bug: python binding for param_get_double reversed value and size.
- Bug: fixed rparam save/load requests - mixing file-id and table-id.
- Bug: fixed bug in index'ing arrays using auot-generated macros, if the index is something like 'x + y'
- Feature: added new rparam load/save from/to named stores.
- Improvement: Updated documentation.

**4.5.1 (2018-11-21)**

- Breaking: removed 'rparam clear' functionality.
- Improvement: rparam.py json matches param specifications (mem_id->id, array_size->array-size)
- Bug: Table 0 was not always read correctly through the Python API.

**4.4.1 (2018-09-20)**

- Bug: prevent stripping leading spaces, when setting string parameters.
- Feature: Added gs_param_table_instance_alloc() for allocating a table instance.

**4.3.1 (2018-08-13)**

- Feature: command: "rparam download" table-specification, only save if current work directory is defined (gs_getcwd()).
- Breaking: rparam and serialize API. REfactored to support checksum.
- Breaking: rparam command - auto-send changed to default false.
- Breaking: moved rparam query API internally for now - only used by rparam commands.
- Feature: added support for showing float/double using scientific notation.

**4.2.2 (2018-05-30)**

- Breaking: Renamed Parameter IO API to Parameter Protocol (PP).
- Bug: Fixed potential crash issue if parameter name to long, missing NULL termination.
