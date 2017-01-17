######################################
#
# dexed
#
######################################

DEXED_DEPENDENCIES = lvtk
DEXED_VERSION = b579f7b8c94b85cbabcd3cb2e532a7908720a218
DEXED_BUNDLES = dexed.lv2
DEXED_SITE = $(call github,dcoredump,dexed,$(DEXED_VERSION))

define DEXED_BUILD_CMDS
        (cd $(@D)/src && $(TARGET_MAKE_ENV) make MOD=1 -j $(PARALLEL_JOBS))
endef

define DEXED_INSTALL_TARGET_CMDS
        (cd $(@D)/src; cp -R dexed.lv2 $(TARGET_DIR)/usr/lib/lv2)
endef

$(eval $(generic-package))

