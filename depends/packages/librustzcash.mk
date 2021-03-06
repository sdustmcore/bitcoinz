package=librustzcash
$(package)_version=0.1
$(package)_download_path=https://github.com/zcash/$(package)/archive/
$(package)_file_name=$(package)-$($(package)_git_commit).tar.gz
$(package)_download_file=$($(package)_git_commit).tar.gz
$(package)_sha256_hash=a5760a90d4a1045c8944204f29fa2a3cf2f800afee400f88bf89bbfe2cce1279
$(package)_git_commit=91348647a86201a9482ad4ad68398152dc3d635e
$(package)_dependencies=rust

#  cargo build --release
define $(package)_build_cmds
    cargo  build --release --lib --target="x86_64-pc-windows-gnu"
endef

#  cp target/release/librustzcash.a $($(package)_staging_dir)$(host_prefix)/lib/ && \
#

define $(package)_stage_cmds
  mkdir $($(package)_staging_dir)$(host_prefix)/lib/ && \
  mkdir $($(package)_staging_dir)$(host_prefix)/include/ && \
  cp target/x86_64-pc-windows-gnu/release/rustzcash.lib $($(package)_staging_dir)$(host_prefix)/lib/librustzcash.a && \
  cp include/librustzcash.h $($(package)_staging_dir)$(host_prefix)/include/
endef
