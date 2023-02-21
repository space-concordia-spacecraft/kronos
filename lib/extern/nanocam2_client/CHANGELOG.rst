NanoCam C1U (nanocam2_client)
=============================

**5.0.5 (2020-08-26)**

- Improvement: added RAW description in API section of documentation

**5.0.4 (2020-02-21)**

- Improvement: updated documentation for gain-global usage

**5.0.3 (2020-02-19)**

- Bug: cam snap missing width and height in help text

**5.0.2 (2019-10-28)**

- Bug: cam peek, use smallest value of CSP_CMP_PEEK_MAX_LEN and GS_CSP_DEFAULT_MAX_PAYLOAD_SIZE

**5.0.1 (2019-10-25)**

- Improvement: added retry count in cam peek command
- Improvement: updated build scripts and dependencies
- Improvement: remove wscript option, not needed anymore since commands must be registered.
- Improvement: added command register function to public header file.
- Improvement: replaced sscanf / command_args(ctx), with gs_string_to_xxx()
- Improvement: use mandatory_args and/or optional_args, instead of checking for minimum args in functions
- Improvement: use new GS_CSP_DEFAULT_MAX_PAYLOAD_SIZE (libgscsp), instead of CSP_CMP_PEEK_MAX_LEN
- Bug: cam peek now only enabled for linux as it does not work for freertos
- Improvement: register write function, now reads back the value and print it
- Improvement: libgscsp updated to 2.7.2

**5.0.0 (2019-09-19)**

- Breaking: switched to libparam 4.7 (new param layout)
- Breaking: renamed kiss-interface to kiss-device in table 0

**4.16.6 (2019-07-03)**

- Improvement: Changed command/gosh definitions to const.

**4.16.5 (2019-05-20)**

- Improvement: Updated command help text.

**4.16.4 (2019-02-21)**

- Improvement: Updated documentation.

**4.16.3 (2018-12-20)**

- Improvement: Updated documentation.

**4.16.2 (2018-11-22)**

- Improvement: Updated dependencies.
