//
//  PathManager.mm
//  CoreLib
//
//  Created by Manuel Petit on 5/14/14.
//
//

#import "PathManager.h"


static NSString const *logdir_stem_key    = @"LogPathStem";
static NSString const *blobdir_stem_key   = @"BlobPathStem";
static NSString const *configdir_stem_key = @"ConfigPathStem";

static NSString const *logdir_in_vault_key    = @"VaultLog";
static NSString const *blobdir_in_vault_key   = @"VaultBlob";
static NSString const *configdir_in_vault_key = @"VaultConfig";

static NSString    *priv_logdir_stem     = @"Logs";
static NSString    *priv_blobdir_stem    = @"Blobs";
static NSString    *priv_configdir_stem  = @"Config";

static BOOL         priv_store_logs_in_vault   = NO;
static BOOL         priv_store_blobs_in_vault  = NO;
static BOOL         priv_store_config_in_vault = NO;


static PathManager *shared_path_manager = 0;


static id pick_one(id proposed, id fallback)
{
	return proposed ? proposed : fallback;
}

static NSString *compose_path(BOOL use_vault, NSString *stem)
{
	if (use_vault) {
		return [@"/vault" stringByAppendingPathComponent:stem];
	}

	if ([stem isAbsolutePath]) {
		return stem;
	}

	NSBundle *mb      = [NSBundle mainBundle];
	NSString *mb_path = [mb bundlePath];
	NSString *mb_home = [mb_path stringByDeletingLastPathComponent];

	return [mb_home stringByAppendingPathComponent:stem];
}


@implementation PathManager

+ (void)initialize
{
	shared_path_manager = [PathManager new];
}

+ (PathManager *)sharedManager
{
	return shared_path_manager;
}

- (id)init
{
	self = [super init];

	if (self) {
		NSBundle     *mb = [NSBundle mainBundle];
		NSURL        *url = [NSURL URLWithString:@"CorelibConfig.plist" relativeToURL:[mb resourceURL]];
		NSDictionary *cfg = [NSDictionary dictionaryWithContentsOfURL:url];
		NSDictionary *pcfg = [cfg objectForKey:@"PathConfig"];

		if (cfg) {
			priv_store_logs_in_vault   = [[pcfg objectForKey:logdir_in_vault_key] boolValue];
			priv_store_blobs_in_vault  = [[pcfg objectForKey:blobdir_in_vault_key] boolValue];
			priv_store_config_in_vault = [[pcfg objectForKey:configdir_in_vault_key] boolValue];

			priv_logdir_stem    = [pick_one([pcfg objectForKey:logdir_stem_key], priv_logdir_stem) retain];
			priv_blobdir_stem   = [pick_one([pcfg objectForKey:blobdir_stem_key], priv_blobdir_stem) retain];
			priv_configdir_stem = [pick_one([pcfg objectForKey:configdir_stem_key], priv_configdir_stem) retain];
		}
	}

	return self;
}

- (NSString *)logPath
{
	return compose_path(priv_store_logs_in_vault, priv_logdir_stem);
}

- (NSString *)blobPath
{
	return compose_path(priv_store_blobs_in_vault, priv_blobdir_stem);
}

- (NSString *)configPath
{
	return compose_path(priv_store_config_in_vault, priv_configdir_stem);
}

@end
