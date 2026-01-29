import re
import os
import yaml

def load_api_docs(yaml_path: str) -> dict:
    with open(yaml_path, 'r') as f:
        content = f.read()
    # YAML does not allow tabs for indentation; normalize to spaces.
    content = content.replace("\t", "    ")
    return yaml.safe_load(content)

def get_api_doc(api_name: str, docs: dict) -> dict | None:
    mapping = {
        "gzopen64": "gzopen",
        "gzseek64": "gzseek",
        "gztell64": "gztell",
        "gzoffset64": "gzoffset",
        "adler32_combine64": "adler32_combine",
        "crc32_combine64": "crc32_combine",
        "crc32_combine_gen64": "crc32_combine_gen",
        "deflateInit2_": "deflateInit",
        "deflateInit2": "deflateInit",
        "inflateInit2_": "inflateInit2",
        "deflateInit_": "deflateInit",
    }
    
    if api_name in mapping:
        api_name = mapping[api_name]
    
    return docs.get(api_name)

def extract_api_calls(c_code: str, zlib_api_list: list[str]) -> set[str]:
    """
    Find all  API calls in the given code
    """
    pattern = re.compile(r'\b([a-zA-Z_][a-zA-Z0-9_]*)\s*\(')
    calls = set(match.group(1) for match in pattern.finditer(c_code))
    return set(calls) & set(zlib_api_list)  
    
def format_api_doc_for_gpt(api_name: str, doc: dict) -> str:
    out = f"API: {api_name}\n\n"
    out += f"Description:\n{doc.get('description', '')}\n\n"

    if 'params' in doc:
        out += "Parameters:\n"
        params = doc['params']
        if isinstance(params, dict):
            for p, desc in params.items():
                out += f"- {p}: {desc}\n"
        elif isinstance(params, list):
            for p in params:
                out += f"- {p}\n"
        out += "\n"

    if 'Returns' in doc:
        out += "Returns:\n"
        returns = doc['Returns']
        if isinstance(returns, dict):
            for r, desc in returns.items():
                out += f"- {r}: {desc}\n"
        elif isinstance(returns, list):
            for item in returns:
                out += f"- {item}\n"
        else:
            out += f"{returns}\n"
        out += "\n"
    if 'example_invariants' in doc:
        out += "example_invariants:\n"
        for inv in doc['example_invariants']:
            out += f"- {inv}\n"
        out += "\n"
    if 'examples' in doc:
        out += "Examples:\n"
        for e in doc['examples']:
            out += f"- {e}\n"
        out += "\n"

    return out
def library_list(library:str) -> list[str]:
    if library == "zlib":
        return ["zlibVersion", "deflateInit", "deflate", "deflateEnd","inflateInit", "inflate", "inflateEnd"
                ,"deflateGetDictionary","deflateSetDictionary","inflateGetDictionary","inflateSetDictionary"
                ,"deflateCopy","deflateReset","deflateParams","deflateTune","deflateBound","deflatePending","deflateUsed",
                "deflatePrime","deflateSetHeader","uncompress","uncompress2","compress","compress2","inflateInit2","inflateSetDictionary","inflateGetDictionary",
                "inflateSync","inflateCopy","inflateReset","inflateReset2","inflateGetHeader","inflatePrime","inflateMark","inflateBackInit","inflateBack","inflateBackEnd","zlibCompileFlags",
                "compressBound","gzopen","gzread","gzwrite","gzclose","gzflush","gzseek","gztell","gzoffset","gzdopen","gzsetparams","gzbuffer","gzfread","gzfwrite","gzprintf",
                "gzgetc","gzrewind","gzeof","gzdirect","gzclose_r","gzclose_w","gzerror","gzclearerr","gzoffset64","gzseek64","gztell64","adler32","adler32_combine",
                "crc32","crc32_combine","crc32_combine_gen","gzopen64","adler32_z","crc32_combine_gen64","adler32_combine64","deflateInit2_","inflateInit2_","inflateResetKeep","deflateInit_"            
                ]
    if library == "re2":
        return ["cre2_version_string", "cre2_version_interface_current", "cre2_version_interface_revision",
                "cre2_version_interface_age", "cre2_opt_new", "cre2_opt_delete", "cre2_opt_set_posix_syntax",
                "cre2_opt_set_longest_match", "cre2_opt_set_log_errors", "cre2_opt_set_literal", "cre2_opt_set_never_nl",
                "cre2_opt_set_dot_nl", "cre2_opt_set_never_capture", "cre2_opt_set_case_sensitive",
                "cre2_opt_set_perl_classes", "cre2_opt_set_word_boundary", "cre2_opt_set_one_line", "cre2_opt_set_max_mem",
                "cre2_opt_set_encoding", "cre2_opt_posix_syntax", "cre2_opt_longest_match", "cre2_opt_log_errors",
                "cre2_opt_literal", "cre2_opt_never_nl", "cre2_opt_dot_nl", "cre2_opt_never_capture",
                "cre2_opt_case_sensitive", "cre2_opt_perl_classes", "cre2_opt_word_boundary", "cre2_opt_one_line",
                "cre2_opt_max_mem", "cre2_opt_encoding", "cre2_new", "cre2_delete", "cre2_pattern", "cre2_error_code",
                "cre2_num_capturing_groups", "cre2_program_size", "cre2_find_named_capturing_groups",
                "cre2_named_groups_iter_new", "cre2_named_groups_iter_next", "cre2_named_groups_iter_delete",
                "cre2_error_string", "cre2_error_arg", "cre2_match", "cre2_easy_match", "cre2_strings_to_ranges",
                "cre2_full_match", "cre2_partial_match", "cre2_consume", "cre2_find_and_consume", "cre2_full_match_re",
                "cre2_partial_match_re", "cre2_consume_re", "cre2_find_and_consume_re", "cre2_replace", "cre2_replace_re",
                "cre2_global_replace", "cre2_global_replace_re", "cre2_extract", "cre2_extract_re", "cre2_quote_meta",
                "cre2_possible_match_range", "cre2_check_rewrite_string", "cre2_set_new", "cre2_set_delete",
                "cre2_set_add", "cre2_set_add_simple", "cre2_set_compile", "cre2_set_match"
                ]
    if library == "cJSON":
        return ["cJSON_Version", "cJSON_InitHooks", "cJSON_Parse", "cJSON_ParseWithLength", "cJSON_ParseWithOpts",
                "cJSON_ParseWithLengthOpts", "cJSON_Print", "cJSON_PrintUnformatted", "cJSON_PrintBuffered",
                "cJSON_PrintPreallocated", "cJSON_Delete", "cJSON_GetArraySize", "cJSON_GetArrayItem",
                "cJSON_GetObjectItem", "cJSON_GetObjectItemCaseSensitive", "cJSON_HasObjectItem", "cJSON_GetErrorPtr",
                "cJSON_GetStringValue", "cJSON_GetNumberValue", "cJSON_IsInvalid", "cJSON_IsFalse", "cJSON_IsTrue",
                "cJSON_IsBool", "cJSON_IsNull", "cJSON_IsNumber", "cJSON_IsString", "cJSON_IsArray", "cJSON_IsObject",
                "cJSON_IsRaw", "cJSON_CreateNull", "cJSON_CreateTrue", "cJSON_CreateFalse", "cJSON_CreateBool",
                "cJSON_CreateNumber", "cJSON_CreateString", "cJSON_CreateRaw", "cJSON_CreateArray", "cJSON_CreateObject",
                "cJSON_CreateStringReference", "cJSON_CreateObjectReference", "cJSON_CreateArrayReference",
                "cJSON_CreateIntArray", "cJSON_CreateFloatArray", "cJSON_CreateDoubleArray", "cJSON_CreateStringArray",
                "cJSON_AddItemToArray", "cJSON_AddItemToObject", "cJSON_AddItemToObjectCS", "cJSON_AddItemReferenceToArray",
                "cJSON_AddItemReferenceToObject", "cJSON_DetachItemViaPointer", "cJSON_DetachItemFromArray",
                "cJSON_DeleteItemFromArray", "cJSON_DetachItemFromObject", "cJSON_DetachItemFromObjectCaseSensitive",
                "cJSON_DeleteItemFromObject", "cJSON_DeleteItemFromObjectCaseSensitive", "cJSON_InsertItemInArray",
                "cJSON_ReplaceItemViaPointer", "cJSON_ReplaceItemInArray", "cJSON_ReplaceItemInObject",
                "cJSON_ReplaceItemInObjectCaseSensitive", "cJSON_Duplicate", "cJSON_Compare", "cJSON_Minify",
                "cJSON_AddNullToObject", "cJSON_AddTrueToObject", "cJSON_AddFalseToObject", "cJSON_AddBoolToObject",
                "cJSON_AddNumberToObject", "cJSON_AddStringToObject", "cJSON_AddRawToObject", "cJSON_AddObjectToObject",
                "cJSON_AddArrayToObject", "cJSON_SetNumberHelper", "cJSON_SetValuestring", "cJSON_malloc", "cJSON_free"
                ]

    if library == "libpng":
        return["png_set_read_status_fn","png_access_version_number","png_create_info_struct","png_create_read_struct","png_create_read_struct_2","png_create_read_struct_2",
        "png_create_write_struct","png_create_write_struct_2","png_data_freer","png_destroy_read_struct","png_destroy_write_struct","png_error_ptr","png_free","png_free_data"
        ,  "png_get_chunk_cache_max", "png_get_chunk_malloc_max", "png_get_copyright", "png_get_filter_type", "png_get_header_ver", "png_get_header_version",
        "png_get_image_height", "png_get_libpng_ver", "png_get_mem_ptr", "png_get_rowbytes", "png_get_rows", "png_get_user_height_max", "png_get_x_pixels_per_meter", "png_image_begin_read_from_memory",
        "png_image_free", "png_image_write_to_file", "png_image_write_to_memory", "png_image_write_to_stdio", "png_init_io", "png_set_IHDR", "png_set_bgr", "png_set_chunk_malloc_max",
        "png_set_compression_buffer_size","png_set_compression_level","png_set_compression_mem_level","png_set_eXIf_1","png_set_expand_gray_1_2_4_to_8","png_set_filler",
        "png_set_filter","png_set_filter_heuristics","png_set_gAMA_fixed","png_set_gamma_fixed","png_set_keep_unknown_chunks","png_set_read_fn","png_set_read_user_chunk_fn",
        "png_set_read_user_transform_fn","png_set_rows","png_set_sig_bytes","png_set_read_fn","png_set_read_status_fn","png_set_read_user_chunk_fn","png_set_rows","png_set_sPLT",
        "png_set_sRGB_gAMA_and_cHRM", "png_set_swap", "png_set_text_compression_method", "png_set_text_compression_window_bits", "png_set_user_limits", "png_set_write_fn", "png_set_write_status_fn",
        "png_warning","png_write_chunk","png_write_chunk_data","png_write_chunk_end","png_write_chunk_start","png_write_image", "png_write_status_ptr"
        ]
    if library == "sqlite3":
        return["sqlite3_bind_blob","sqlite3_blob","sqlite3_blob_bytes","sqlite3_blob_close","sqlite3_blob_open",
        "sqlite3_blob_read","sqlite3_busy_handler","sqlite3_changes","sqlite3_close","sqlite3_close_v2","sqlite3_column_count",
        "sqlite3_column_decltype","sqlite3_column_int64","sqlite3_column_name","sqlite3_column_origin_name16","sqlite3_column_table_name",
        "sqlite3_column_type","sqlite3_compileoption_used","sqlite3_complete","sqlite3_complete16","sqlite3_context","sqlite3_data_count",
        "sqlite3_db_handle","sqlite3_errmsg","sqlite3_exec","sqlite3_expanded_sql","sqlite3_file","sqlite3_finalize","sqlite3_free","sqlite3_get_table",
        "sqlite3_hard_heap_limit64","sqlite3_int64","sqlite3_interrupt","sqlite3_keyword_check","sqlite3_keyword_count","sqlite3_keyword_name","sqlite3_libversion",
        "sqlite3_libversion_number","sqlite3_log","sqlite3_malloc","sqlite3_malloc64","sqlite3_memory_used","sqlite3_mprintf","sqlite3_msize","sqlite3_next_stmt",
        "sqlite3_open","sqlite3_open16","sqlite3_open_v2","sqlite3_prepare","sqlite3_prepare16","sqlite3_prepare16_v3","sqlite3_prepare_v2","sqlite3_prepare_v3",
        "sqlite3_randomness","sqlite3_realloc","sqlite3_realloc64","sqlite3_release_memory","sqlite3_reset","sqlite3_reset_auto_extension","sqlite3_result_int",
        "sqlite3_result_null","sqlite3_result_pointer","sqlite3_shutdown","sqlite3_sleep","sqlite3_snapshot","sqlite3_snapshot_cmp","sqlite3_snapshot_open","sqlite3_snprintf",
        "sqlite3_status","sqlite3_status64","sqlite3_step","sqlite3_stmt","sqlite3_stmt_busy","sqlite3_stmt_explain","sqlite3_stmt_scanstatus_reset","sqlite3_str","sqlite3_str_append",
        "sqlite3_str_appendall","sqlite3_str_appendchar","sqlite3_str_errcode","sqlite3_str_finish","sqlite3_str_new","sqlite3_str_reset","sqlite3_str_value","sqlite3_strglob",
        "sqlite3_stricmp","sqlite3_strnicmp","sqlite3_system_errno","sqlite3_thread_cleanup","sqlite3_threadsafe","sqlite3_uint64","sqlite3_value"
]
    if library == "libpcap":
        script_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        docs = load_api_docs(os.path.join(script_dir, "library", "libpcap.yaml"))
        if isinstance(docs, dict):
            return list(docs.keys())
        return []
    if library =="lcms":
        return ["cmsAllocNamedColorList","cmsBuildGamma","cmsBuildParametricToneCurve","cmsBuildTabulatedToneCurve16","cmsCloseIOhandler","cmsCloseProfile","cmsColorSpaceSignature",
        "cmsCreateBCHSWabstractProfile","cmsCreateBCHSWabstractProfileTHR","cmsCreateContext","cmsCreateInkLimitingDeviceLinkTHR","cmsCreateLab2ProfileTHR","cmsCreateLab4Profile","cmsCreateLab4ProfileTHR",
        "cmsCreateMultiprofileTransformTHR","cmsCreateNULLProfile","cmsCreateNULLProfileTHR","cmsCreateProfilePlaceholder","cmsCreateTransformTHR","cmsCreateXYZProfile","cmsCreateXYZProfileTHR","cmsCreate_sRGBProfile",
        "cmsCreate_sRGBProfileTHR","cmsD50_XYZ","cmsD50_xyY","cmsDeleteContext","cmsDeleteTransform","cmsDetectBlackPoint","cmsDetectDestinationBlackPoint","cmsDupContext","cmsEstimateGamma","cmsEvalToneCurveFloat",
        "cmsFloat2XYZEncoded","cmsFormatterForPCSOfProfile","cmsFreeNamedColorList","cmsFreeToneCurveTriple","cmsGBDAlloc","cmsGetDeviceClass","cmsGetEncodedCMMversion","cmsGetEncodedICCversion","cmsGetHeaderAttributes",
        "cmsGetHeaderCreationDateTime","cmsGetHeaderCreator","cmsGetHeaderFlags","cmsGetHeaderManufacturer","cmsGetHeaderModel","cmsGetHeaderProfileID","cmsGetHeaderRenderingIntent","cmsGetNamedColorList","cmsGetPCS",
        "cmsGetPipelineContextID","cmsGetProfileContextID","cmsGetProfileInfoASCII","cmsGetProfileVersion","cmsGetSupportedIntents","cmsGetSupportedIntentsTHR","cmsGetTagCount","cmsGetTagSignature","cmsGetTransformInputFormat",
        "cmsGetTransformOutputFormat","cmsHANDLE","cmsHPROFILE","cmsHTRANSFORM","cmsIOHANDLER","cmsIT8Alloc","cmsIT8EnumDataFormat","cmsIT8EnumProperties","cmsIT8Free","cmsIT8GetPatchName","cmsIT8SetPropertyStr","cmsIsCLUT",
        "cmsIsIntentSupported","cmsIsMatrixShaper","cmsMD5computeID","cmsMLU","cmsMLUalloc","cmsMLUfree","cmsMLUgetUTF8","cmsMLUgetWide","cmsMLUsetASCII","cmsNAMEDCOLORLIST","cmsNamedColorCount","cmsOpenIOhandlerFromNULL",
        "cmsOpenProfileFromMem","cmsPipeline","cmsPipelineAlloc","cmsPipelineEval16","cmsPipelineEvalFloat","cmsPipelineFree","cmsPipelineGetPtrToFirstStage","cmsPipelineGetPtrToLastStage","cmsPipelineInsertStage",
        "cmsPipelineOutputChannels","cmsPipelineSetSaveAs8bitsFlag","cmsPipelineUnlinkStage","cmsProfileClassSignature","cmsReadTag","cmsSaveProfileToFile","cmsSaveProfileToIOhandler","cmsSaveProfileToMem",
        "cmsSetAlarmCodes","cmsSetAlarmCodesTHR","cmsSetEncodedICCversion","cmsSetHeaderFlags","cmsSetHeaderRenderingIntent","cmsSetProfileVersion","cmsStage","cmsStageAllocCLut16bit","cmsStageAllocCLutFloat",
        "cmsStageAllocIdentity","cmsStageAllocMatrix","cmsStageAllocToneCurves","cmsStageData","cmsStageDup","cmsStageFree","cmsStageInputChannels","cmsStageLoc","cmsStageNext","cmsStageOutputChannels","cmsStageSignature",
        "cmsStageType","cmsTagSignature","cmsTempFromWhitePoint","cmsToneCurve","cmsTransform2DeviceLink","cmsUnregisterPlugins","cmsXYZ2Lab","cmsxyY2XYZ"]
    if library =="libaom":
        return["aom_codec_av1_dx","aom_codec_dec_init_ver","aom_codec_decode","aom_codec_destroy","aom_codec_error","aom_codec_error_detail","aom_codec_peek_stream_info","aom_codec_set_frame_buffer_functions",
        "aom_img_alloc","aom_img_alloc_with_border","amo_img_free","aom_uleb_encode"]