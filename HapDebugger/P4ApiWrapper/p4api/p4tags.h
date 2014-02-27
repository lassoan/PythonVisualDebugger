/*
 * Copyright 1995, 2000 Perforce Software.  All rights reserved.
 *
 * This file is part of Perforce - the FAST SCM System.
 */

/* 
 * p4tags.h - definition of rpc variable names
 *
 * The P4Tag class contains nothing but static variables containing
 * the strings passed to the Rpc::SetVar() and Invoke() functions, to
 * save string space across files.
 */

class P4Tag {

    public:

	// client service methods

	static const char c_Ack[];
	static const char c_CheckFile[];
	static const char c_ChmodFile[];
	static const char c_CloseDiff[];
	static const char c_CloseFile[];
	static const char c_CloseMerge[];
	static const char c_Crypto[];
	static const char c_DeleteFile[];
	static const char c_EditData[];
	static const char c_ErrorPause[];
	static const char c_FstatInfo[];
	static const char c_HandleError[];
	static const char c_InputData[];
	static const char c_OpenDiff[];
	static const char c_OpenFile[];
	static const char c_OpenMerge2[];
	static const char c_OpenMerge3[];
	static const char c_OutputBinary[];
	static const char c_OutputData[];
	static const char c_OutputError[];
	static const char c_OutputInfo[];
	static const char c_OutputText[];
	static const char c_Prompt[];
	static const char c_ScanDir[];
	static const char c_SendFile[];
	static const char c_SetPassword[];
	static const char c_WriteDiff[];
	static const char c_WriteFile[];
	static const char c_WriteMerge[];

	// variables known to the clients

	static const char v_action[];
	static const char v_baseName[];
	static const char v_bits[];
	static const char v_change[];
	static const char v_client[];
	static const char v_clientFile[];
	static const char v_commit[];
	static const char v_compare[];
	static const char v_confirm[];
	static const char v_cwd[];
	static const char v_data[];
	static const char v_decline[];
	static const char v_depotFile[];
	static const char v_desc[];
	static const char v_diffFlags[];
	static const char v_digest[];
	static const char v_dir[];
	static const char v_fatal[];
	static const char v_file[];
	static const char v_fileSize[];
	static const char v_func[];
	static const char v_forceType[];
	static const char v_handle[];
	static const char v_haveRev[];
	static const char v_headAction[];
	static const char v_headChange[];
	static const char v_headRev[];
	static const char v_headTime[];
	static const char v_headType[];
	static const char v_host[];
	static const char v_level[];
	static const char v_mergeAuto[];
	static const char v_mergeConfirm[];
	static const char v_mergeDecline[];
	static const char v_mergeHow[];
	static const char v_mergePerms[];
	static const char v_noBase[];
	static const char v_noclobber[];
	static const char v_noecho[];
	static const char v_open[];
	static const char v_os[];
	static const char v_otherLock[];
	static const char v_otherOpen[];
	static const char v_ourLock[];
	static const char v_path[];
	static const char v_perms[];
	static const char v_rev[];
	static const char v_rmdir[];
	static const char v_showAll[];
	static const char v_specdef[];
	static const char v_status[];
	static const char v_tag[];
	static const char v_theirName[];
	static const char v_time[];
	static const char v_token[];
	static const char v_type[];
	static const char v_unmap[];
	static const char v_unresolved[];
	static const char v_user[];
	static const char v_warning[];
	static const char v_write[];
	static const char v_yourName[];

	// server-to-server variables

	static const char v_asBinary[];		// p4 resolve
	static const char v_attrib[];		// p4 index
	static const char v_baseDepotRec[];	// p4 resolve
	static const char v_changeNo[];		// p4 submit 
	static const char v_fixStatus[];	// p4 change
	static const char v_data2[];		// p4 passwd
	static const char v_delete[];		// p4 index
	static const char v_depotRec[];
	static const char v_do[];		// p4 diff
	static const char v_haveRec[];		// p4 sync
	static const char v_integRec[];
	static const char v_keyVal[];		// p4 index
	static const char v_lbrFile[];		// remote depot
	static const char v_lbrRev[];		// remote depot
	static const char v_lbrType[];		// remote depot
	static const char v_message[];		// p4 diff
	static const char v_noretry[];		// p4 specs
	static const char v_table[];		// remote depot
	static const char v_value[];		// p4 index
	static const char v_workRec[];

	// functions that can be enaced on the server
	
	static const char u_add[];		// Open a new file to add it to the depot
	static const char u_admin[];		// Perform administrative operations on the server
	static const char u_branch[];		// Create or edit a branch specification
	static const char u_branches[];		// Display list of branches"
	static const char u_change[];		// Create or edit a changelist description
	static const char u_changes[];		// Display list of pending and submitted changelists
	static const char u_client[];		// Create or edit a client specification and its view
	static const char u_clients[];		// Display list of known clients
	static const char u_counter[];		// Display, set, or delete a counter
	static const char u_counters[];		// Display list of known counters
	static const char u_delete[];		// Open an existing file to delete it from the depot
	static const char u_depot[];		// Create or edit a depot specification
	static const char u_depots[];		// Display list of depots
	static const char u_describe[];		// Display a changelist description
	static const char u_diff[];		// Display diff of client file with depot file
	static const char u_diff2[];		// Display diff of two depot files
	static const char u_dirs[];		// List subdirectories of a given depot directory
	static const char u_edit[];		// Open an existing file for edit
	static const char u_filelog[];		// List revision history of files
	static const char u_files[];		// List files in the depot
	static const char u_fix[];		// Mark jobs as being fixed by named changelists
	static const char u_fixes[];		// List what changelists fix what job
	static const char u_flush[];		// Fake a 'p4 sync' by not moving files
	static const char u_fstat[];		// Dump file info
	static const char u_group[];		// Change members of a user group
	static const char u_groups[];		// List groups (of users)
	static const char u_have[];		// List revisions last synced
	static const char u_help[];		// Print this help message
	static const char u_info[];		// Print out client/server information
	static const char u_integrate[];	// Schedule integration from one file to another
	static const char u_integrated[];	// Show integrations that have been submitted
	static const char u_job[];		// Create or edit a job (defect) specification
	static const char u_jobs[];		// Display list of jobs
	static const char u_jobspec[];		// Edit the job template
	static const char u_label[];		// Create or edit a label specification and its view
	static const char u_labels[];		// Display list of labels
	static const char u_labelsync[];	// Synchronize label with the current client contents
	static const char u_lock[];		// Lock an opened file against changelist submission
	static const char u_obliterate[];	// Remove files and their history from the depot
	static const char u_opened[];		// Display list of files opened for pending changelist
	static const char u_passwd[];		// Set the user's password on the server (and Windows client)
	static const char u_print[];		// Retrieve a depot file to the standard output
	static const char u_protect[];		// Modify protections in the server namespace
	static const char u_rename[];		// Explains how to rename files
	static const char u_reopen[];		// Change the type or changelist number of an opened file
	static const char u_resolve[];		// Merge open files with other revisions or files
	static const char u_resolved[];		// Show files that have been merged but not submitted
	static const char u_revert[];		// Discard changes from an opened file
	static const char u_review[];		// List and track changelists (for the review daemon)
	static const char u_reviews[];		// Show what users are subscribed to review files
	static const char u_set[];		// Set variables in the registry (Windows only)
	static const char u_submit[];		// Submit open files to the depot
	static const char u_sync[];		// Synchronize the client with its view of the depot
	static const char u_triggers[];		// Modify list of pre-submit triggers
	static const char u_typemap[];		// Modify the file name-to-type mapping table
	static const char u_unlock[];		// Release a locked file but leave open
	static const char u_user[];		// Create or edit a user specification
	static const char u_users[];		// Display list of known users
	static const char u_verify[];		// Verify that the server archives are intact
	static const char u_where[];		// Show how file names map through the client view


} ;
