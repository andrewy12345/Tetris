# add IP repos
set_property  ip_repo_paths  [file dirname [get_property DIRECTORY [current_project]]]/hardware/ip [current_project]
update_ip_catalog

# add constraints files
add_files -fileset constrs_1  [file dirname [get_property DIRECTORY [current_project]]]/hardware/constraints/adventures_with_ip.xdc
add_files -fileset constrs_1  [file dirname [get_property DIRECTORY [current_project]]]/hardware/constraints/zedboard_master.xdc

# rebuild the project hardware
source [file dirname [get_property DIRECTORY [current_project]]]/hardware/block_design.tcl

# generate block design
generate_target all [get_files  [get_property DIRECTORY [current_project]]/[current_project]/[current_project].srcs/sources_1/bd/design_1/design_1.bd]

# add wrapper
make_wrapper -files [get_files [get_property DIRECTORY [current_project]]/[current_project].srcs/sources_1/bd/design_1/design_1.bd] -top

# launch synthesis and implementation
add_files -norecurse [get_property DIRECTORY [current_project]]/[current_project].gen/sources_1/bd/design_1/hdl/design_1_wrapper.v
reset_run synth_1
reset_run impl_1
launch_runs synth_1
launch_runs impl_1  # -to_step write_bitstream