######################################
#
# dexed
#
######################################

DEXED_DEPENDENCIES = lvtk
DEXED_VERSION = e95b8626cd2a6a3175df5238ca4efb05e470e4ef
DEXED_BUNDLES = dexed.lv2
DEXED_SITE = $(call github,dcoredump,dexed,$(DEXED_VERSION))
DEXED_TARGET_MAKE = $(TARGET_MAKE_ENV) $(TARGET_CONFIGURE_OPTS) $(MAKE) MOD=1

define DEXED_BUILD_CMDS
        (cd $(@D)/src && $(DEXED_TARGET_MAKE) -j $(PARALLEL_JOBS))
endef

define DEXED_INSTALL_TARGET_CMDS
        (cd $(@D)/src; cp -R dexed.lv2 $(TARGET_DIR)/usr/lib/lv2)
endef

$(eval $(generic-package))

