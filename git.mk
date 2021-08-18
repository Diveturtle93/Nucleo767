# recompile version.h dependants when GIT_VERSION changes, uses temporary file version~
.PHONY: force
../Application/Inc/git.h: force
	@echo "#ifndef _GIT_H_ /* Definiere Klasse _GIT_H_ */" > $@
	@echo "#define _GIT_H_ /* GIT_H */" >> $@
	@echo "" >> $@
	@echo "#define GIT_COMMIT \"$(shell git describe --always)\"" >> $@
	@echo "#define GIT_BRANCH \"$(shell git branch --show-current)\"" >> $@
	@echo "#define GIT_HASH \"$(shell git rev-parse HEAD)\"" >> $@
	@echo "#define GIT_LAST_TAG \"$(shell git describe abbrev=0 --tags)\"" >> $@
	@echo "#define GIT_TAG_DIRTY \"$(shell git describe)\" >> $@
	@echo "#define GIT_TAG_DIRTY_NUMBER \"$(shell git shortlog -s)\" >> $@
	@echo "" >> $@
	@echo "#define BUILD_DATE \"$(shell date +"%Y-%m-%d")\"" >> $@
	@echo "#define BUILD_TIME \"$(shell date +"%H:%M:%S")\"" >> $@
	@echo "#endif /* GIT_H */" >> $@