# CAPSTONE PROJECT
Manages a specified system directory. If a directory is above a particular size/content [configurable, known as `monitoring policy`] , execute a `deletion policy`.

## Workflow
Get `directory path`, `monitoring policy`, and `deletion policy`
Check if directory exist.
If directory does not exist, indicate directory does not exist and gracefully terminate 
If directory exists, start monitoring loop.

### Monitoring loop
Gets `monitoring policy` (max directory size or max number of content)
Checks if directory is obeying `monitoring policy`
If deviating from `monitoring policy`
	- enforce `deletion policy`
	- send a message to kernel
NICE TO HAVE --> send a message to TELEGRAF


Path
	Should help to get the system directory path. Does this by
		- Gets the path to the system directory
		- Confirming the existence of the directory


