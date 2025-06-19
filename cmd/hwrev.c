/*
 * (C) Copyright 2020 Hardkernel Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <adc.h>

#define check_range(min,max,val) (val > 0 && val > min && val < max ? 1 : 0)

int do_hwrev(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned int hwrev_adc;

	if (adc_channel_single_shot("saradc", 0, &hwrev_adc)) {
		printf("board hw rev failed\n");
		return CMD_RET_FAILURE;
	}
#if 1
	/* RG351MP */
	if (check_range(146, 186, hwrev_adc)) {
		env_set("hwrev", "rg351mp");
		env_set("devnum", "1");
		run_command("mmc rescan; mmc list; mmc dev 1; mmc info", 0);
		run_command("setenv -f dtb_name \"${PanelPathSlash}rg351mp-kernel.dtb\"", 0);
		run_command("printenv; sleep 20", 0);
		
		 /* run_command("setenv -f PanChoEnvSize 0x1000; setenv -f PanChoEnvLoc 28560", 0); seems clear on uboot.bin mbr and gpt
		run_command("setenv -f WipePanChoEnv \"mmc erase ${PanChoEnvLoc} ${PanChoEnvSize}\";", 0); 
		run_command("setenv -f LoadPanChoEnv \"mmc read 0x2000000 ${PanChoEnvLoc} ${PanChoEnvSize};env import -b 0x2000000 ${PanChoEnvSize}\";", 0); 
		run_command("setenv -f SavePanChoEnv \"env export -b -s ${PanChoEnvSize} 0x2000000; mmc write 0x2000000 ${PanChoEnvLoc} ${PanChoEnvSize}\"", 0); 
		run_command("echo \"###### before load ######\" ;printenv PanChoEnvSize WipePanChoEnv LoadPanChoEnv SavePanChoEnv PanChoEnvLoaded", 0); 
		
		run_command("setenv -f PanChoEnvLoaded false; run LoadPanChoEnv; echo \"###### after load ######\"; printenv PanChoEnvSize WipePanChoEnv LoadPanChoEnv SavePanChoEnv PanChoEnvLoaded", 0);
		
		run_command("if ${PanChoEnvLoaded}; then; echo PanChoEnv Loaded; else; if load mmc 1:1 0x02000000 rg351mp-kernel.dtb; then; echo \"found rg351mp-kernel.dtb, business as usual\"; else; echo \"rg351mp-kernel.dtb not, uninitialized PanCho?\"; if load mmc 1:1 0x02000000 \"ScreenFiles/Panel 4/rg351mp-kernel.dtb\"; then; echo \"found ScreenFiles, lets temporarily load default (pan 4) so PanCho can init\"; setenv PanelPathSlash \"ScreenFiles/Panel 4/\"; else; echo \"cant find r36s dtb anywhere expected, boot will fail so lets fail early and sleep for a while to avoid lots of looping\"; sleep 60; reset; fi; fi; fi", 0);
		
		run_command("setenv -f dtb_name \"${PanelPathSlash}rg351mp-kernel.dtb\"", 0); */
	}
	/* RG351V */
	else if (check_range(494, 534, hwrev_adc)) {
		env_set("hwrev", "rg351v");
		env_set("dtb_name", "rk3326-rg351v-linux.dtb");
	}
	/* RG351P */
	else if (check_range(655, 695, hwrev_adc)) {
		env_set("hwrev", "rg351p");
		env_set("dtb_name", "rk3326-rg351p-linux.dtb");
	}
	/* Unknown */
	else {
		env_set("hwrev", "v00");
		env_set("dtb_name", "rk3326-rg351mp-linux.dtb");
	}
#else
		env_set("hwrev", "rg351mp");
		env_set("dtb_name", "rk3326-rg351mp-linux.dtb");

#endif
	printf("adc0 (hw rev) %d\n", hwrev_adc);
	printf("Model = %s\n",env_get("hwrev"));
	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	hwrev, 1, 1, do_hwrev,
	"check hw revision of OGA",
	""
);
