/* Auto-generated config file hpl_mcan_config.h */
#ifndef HPL_MCAN_CONFIG_H
#define HPL_MCAN_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

#ifndef CONF_CAN0_ENABLED
#define CONF_CAN0_ENABLED 1
#endif
// <h> Basic Configuration

// <o> CAN Mode Enable
// <i> decide whether enable CAN FD mode or bit rate switching
// <0=> ISO11898_1
// <1=> FD
// <2=> FD with Bitrate Switch
// <id> can_cccr_cme
#ifndef CONF_CAN0_CCCR_CME
#define CONF_CAN0_CCCR_CME 0
#endif

// <o> CAN Mode Request (read/write)
// <i> decide which request CAN mode
// <0=> NO_CHANGE
// <1=> FD
// <2=> FD with Bitrate Switch
// <3=> ISO11898_1
// <id> can_cccr_cmr
#ifndef CONF_CAN0_CCCR_CMR
#define CONF_CAN0_CCCR_CMR 0
#endif

// </h>

// <h> Normal Bit Timing and Prescaler Configuration

// <o> Baud Rate Prescaler <1-1024>
// <i> The value by which the oscillator frequency is divided for generating the CAN time quanta. CAN time quantum (tq) = (This value) x CAN core clock cycles (tcore clock)
// <id> can_btp_brp
#ifndef CONF_CAN0_BTP_BRP
#define CONF_CAN0_BTP_BRP 3
#endif

// <o> time segment before sample point <2-64>
// <i> The duration of time segment = (This value) x CAN time quantum (tq)
// <id> can_btp_tseg1
#ifndef CONF_CAN0_BTP_TSEG1
#define CONF_CAN0_BTP_TSEG1 11
#endif

// <o> time segment after sample point <1-16>
// <i> The duration of time segment = (This value) x CAN time quantum (tq)
// <id> can_btp_tseg2
#ifndef CONF_CAN0_BTP_TSEG2
#define CONF_CAN0_BTP_TSEG2 8
#endif

// <o> Data (Re)Syncronization Jump Width <1-16>
// <i> The duration of a synchronization jump = (This value) x CAN time quantum (tq)
// <id> can_btp_sjw
#ifndef CONF_CAN0_BTP_SJW
#define CONF_CAN0_BTP_SJW 4
#endif

// </h>
// <h> Fast Bit Timing and Prescaler Configuration

// <o> Fast Baud Rate Prescaler <1-32>
// <i> The value by which the peripheral clock is divided for generating the bit time quanta. CAN time quantum (tq) = (This value) x CAN core clock cycles (tcore clock)
// <id> can_fbtp_fbrp
#ifndef CONF_CAN0_FBTP_FBRP
#define CONF_CAN0_FBTP_FBRP 1
#endif

// <o> Fast time segment before sample point <2-16>
// <i> The duration of time segment = (This value) x CAN time quantum (tq)
// <id> can_fbtp_ftseg1
#ifndef CONF_CAN0_FBTP_FTSEG1
#define CONF_CAN0_FBTP_FTSEG1 10
#endif

// <o> Fast time segment after sample point <1-8>
// <i> The duration of time segment = (This value) x CAN time quantum (tq)
// <id> can_fbtp_ftseg2
#ifndef CONF_CAN0_FBTP_FTSEG2
#define CONF_CAN0_FBTP_FTSEG2 4
#endif

// <o> Fast (Re)Syncronization Jump Width <1-4>
// <i> The duration of a synchronization jump = (This value) x CAN time quantum (tq)
// <id> can_fbtp_fsjw
#ifndef CONF_CAN0_FBTP_FSJW
#define CONF_CAN0_FBTP_FSJW 4
#endif

// <q> Transceiver Delay Compensation Enable
// <i> Transceiver Delay Compensation Enable
// <id> can_fbtp_tdc
#ifndef CONF_CAN0_FBTP_TDC
#define CONF_CAN0_FBTP_TDC 0
#endif

// <o> Transceiver Delay Compensation Offset <0-31>
// <i> Transceiver Delay Compensation Offset
// <id> can_fbtp_tdco
#ifndef CONF_CAN0_FBTP_TDCO
#define CONF_CAN0_FBTP_TDCO 11
#endif

// </h>

// <h> RX FIFO Configuration

// <o> Operation Mode
// <i> Select Operation Mode
// <0=> blocking mode
// <1=> overwrite mode
// <id> can_rxf0c_f0om
#ifndef CONF_CAN0_RXF0C_F0OM
#define CONF_CAN0_RXF0C_F0OM 0
#endif

// <o> Watermark <0-64>
// <i> Watermark, 0 for disable watermark interrupt
// <id> can_rxf0c_f0wm
#ifndef CONF_CAN0_RXF0C_F0WM
#define CONF_CAN0_RXF0C_F0WM 0
#endif

// <o> Size <0-64>
// <i> Number of Rx FIFO 0 element
// <id> can_rxf0c_f0s
#ifndef CONF_CAN0_RXF0C_F0S
#define CONF_CAN0_RXF0C_F0S 6
#endif

// <o> Data Field Size
// <i> Rx FIFO 0 Data Field Size
// <0=> 8 byte data field.
// <1=> 12 byte data field.
// <2=> 16 byte data field.
// <3=> 20 byte data field.
// <4=> 24 byte data field.
// <5=> 32 byte data field.
// <6=> 48 byte data field.
// <7=> 64 byte data field.
// <id> can_rxesc_f0ds
#ifndef CONF_CAN0_RXESC_F0DS
#define CONF_CAN0_RXESC_F0DS 0
#endif

/* Bytes size for CAN FIFO 0 element, plus 8 bytes for R0,R1 */
#undef CONF_CAN0_F0DS
#define CONF_CAN0_F0DS                                                                                                 \
	((CONF_CAN0_RXESC_F0DS < 5) ? ((CONF_CAN0_RXESC_F0DS << 2) + 16) : (40 + ((CONF_CAN0_RXESC_F0DS % 5) << 4)))

// </h>

// <h> TX FIFO Configuration

// <o> Transmit FIFO Size <0-32>
// <i> Number of Tx Buffers used for Tx FIFO
// <id> can_txbc_tfqs
#ifndef CONF_CAN0_TXBC_TFQS
#define CONF_CAN0_TXBC_TFQS 6
#endif

// <o> Tx Buffer Data Field Size
// <i> Tx Buffer Data Field Size
// <0=> 8 byte data field.
// <1=> 12 byte data field.
// <2=> 16 byte data field.
// <3=> 20 byte data field.
// <4=> 24 byte data field.
// <5=> 32 byte data field.
// <6=> 48 byte data field.
// <7=> 64 byte data field.
// <id> can_txesc_tbds
#ifndef CONF_CAN0_TXESC_TBDS
#define CONF_CAN0_TXESC_TBDS 0
#endif

/* Bytes size for CAN Transmit Buffer element, plus 8 bytes for R0,R1 */
#undef CONF_CAN0_TBDS
#define CONF_CAN0_TBDS                                                                                                 \
	((CONF_CAN0_TXESC_TBDS < 5) ? ((CONF_CAN0_TXESC_TBDS << 2) + 16) : (40 + ((CONF_CAN0_TXESC_TBDS % 5) << 4)))

// </h>

// <h> TX Event Configuration

// <o> Watermark <0-32>
// <i> Watermark, 0 for disable watermark interrupt
// <id> can_txefc_efwm
#ifndef CONF_CAN0_TXEFC_EFWM
#define CONF_CAN0_TXEFC_EFWM 0
#endif

// <o> Size <0-32>
// <i> Number of Event FIFO element
// <id> can_txefc_efs
#ifndef CONF_CAN0_TXEFC_EFS
#define CONF_CAN0_TXEFC_EFS 2
#endif

// </h>

// <h> Filter Configuration

// <o> Accept Non-matching Frames Standard
// <i> Defines how received messages with 11-bit IDs that do not match any
// <i> element of the filter list are treated.
// <0=> Accept in Rx FIFO 0
// <1=> Accept in Rx FIFO 1
// <2=> Reject
// <id> can_gfc_anfs
#ifndef CONF_CAN0_GFC_ANFS
#define CONF_CAN0_GFC_ANFS 2
#endif

// <o> Accept Non-matching Frames Extended
// <i> Defines how received messages with 29-bit IDs that do not match any
// <i> element of the filter list are treated.
// <0=> Accept in Rx FIFO 0
// <1=> Accept in Rx FIFO 1
// <2=> Reject
// <id> can_gfc_anfe
#ifndef CONF_CAN0_GFC_ANFE
#define CONF_CAN0_GFC_ANFE 2
#endif

// <o> Reject Remote Frames Standard
// <i> Defines how received remote frames with 11-bit standard IDs.
// <0=> Filter remote frames with 11-bit standard IDs
// <1=> Reject all remote frames with 11-bit standard IDs
// <id> can_gfc_rrfs
#ifndef CONF_CAN0_GFC_RRFS
#define CONF_CAN0_GFC_RRFS 1
#endif

// <o> Reject Remote Frames Extended
// <i> Defines how received remote frames with 29-bit standard IDs.
// <0=> Filter remote frames with 29-bit standard IDs
// <1=> Reject all remote frames with 29-bit standard IDs
// <id> can_gfc_rrfe
#ifndef CONF_CAN0_GFC_RRFE
#define CONF_CAN0_GFC_RRFE 1
#endif

// <o> Number of standard Message ID filter elements <0-128>
// <i> Number of standard Message ID filter elements
// <id> can_sidfc_lss
#ifndef CONF_CAN0_SIDFC_LSS
#define CONF_CAN0_SIDFC_LSS 2
#endif

// <o> Number of Extended Message ID filter elements <0-64>
// <i> Number of Extended Message ID filter elements
// <id> can_xidfc_lss
#ifndef CONF_CAN0_XIDFC_LSS
#define CONF_CAN0_XIDFC_LSS 2
#endif

// <o> Extended ID Mask <0x0000-0x1FFFFFFF>
// <i> For acceptance filtering of extended frames the Extended ID AND Mask is
// <i> ANDed with the Message ID of a received frame. Intended for masking of
// <i> 29-bit IDs in SAE J1939. With the reset value of all bits set to one the
// <i> mask is not active.
// <id> can_xidam_eidm
#ifndef CONF_CAN0_XIDAM_EIDM
#define CONF_CAN0_XIDAM_EIDM 0x0
#endif

// </h>

// <h> Interrupt Configuration

// <q> Error Warning
// <i> Indicates whether to not disable CAN error warning interrupt
// <id> can_ie_ew
#ifndef CONF_CAN0_IE_EW
#define CONF_CAN0_IE_EW 0
#endif

// <q> Error Active
// <i> Indicates whether to not disable CAN error active interrupt
// <id> can_ie_ea
#ifndef CONF_CAN0_IE_EA
#define CONF_CAN0_IE_EA 0
#endif

// <q> Error Passive
// <i> Indicates whether to not disable CAN error passive interrupt
// <id> can_ie_ep
#ifndef CONF_CAN0_IE_EP
#define CONF_CAN0_IE_EP 0
#endif

// <q> Bus Off
// <i> Indicates whether to not disable CAN bus off interrupt
// <id> can_ie_bo
#ifndef CONF_CAN0_IE_BO
#define CONF_CAN0_IE_BO 0
#endif

// <q> Data Overrun
// <i> Indicates whether to not disable CAN data overrun interrupt
// <id> can_ie_do
#ifndef CONF_CAN0_IE_DO
#define CONF_CAN0_IE_DO 0
#endif

// </h>

#ifndef CONF_CAN0_CCCR_REG
#define CONF_CAN0_CCCR_REG (CONF_CAN0_CCCR_CME << MCAN_CCCR_CME_Pos) | (CONF_CAN0_CCCR_CMR << MCAN_CCCR_CMR_Pos)
#endif

#ifndef CONF_CAN0_BTP_REG
#define CONF_CAN0_BTP_REG                                                                                              \
	MCAN_BTP_BRP(CONF_CAN0_BTP_BRP - 1) | MCAN_BTP_TSEG1(CONF_CAN0_BTP_TSEG1 - 1)                                      \
	    | MCAN_BTP_TSEG2(CONF_CAN0_BTP_TSEG2 - 1) | MCAN_BTP_SJW(CONF_CAN0_BTP_SJW - 1)
#endif

#ifndef CONF_CAN0_FBTP_REG
#define CONF_CAN0_FBTP_REG                                                                                             \
	(CONF_CAN0_FBTP_TDC << MCAN_FBTP_TDC_Pos) | MCAN_FBTP_FBRP(CONF_CAN0_FBTP_FBRP - 1)                                \
	    | MCAN_FBTP_FTSEG1(CONF_CAN0_FBTP_FTSEG1 - 1) | MCAN_FBTP_FTSEG2(CONF_CAN0_FBTP_FTSEG2 - 1)                    \
	    | MCAN_FBTP_FSJW(CONF_CAN0_FBTP_FSJW - 1) | MCAN_FBTP_TDCO(CONF_CAN0_FBTP_TDCO)
#endif

#ifndef CONF_CAN0_RXF0C_REG
#define CONF_CAN0_RXF0C_REG                                                                                            \
	(CONF_CAN0_RXF0C_F0OM << MCAN_RXF0C_F0OM_Pos) | MCAN_RXF0C_F0WM(CONF_CAN0_RXF0C_F0WM)                              \
	    | MCAN_RXF0C_F0S(CONF_CAN0_RXF0C_F0S)
#endif

#ifndef CONF_CAN0_RXESC_REG
#define CONF_CAN0_RXESC_REG MCAN_RXESC_F0DS(CONF_CAN0_RXESC_F0DS)
#endif

#ifndef CONF_CAN0_TXESC_REG
#define CONF_CAN0_TXESC_REG MCAN_TXESC_TBDS(CONF_CAN0_TXESC_TBDS)
#endif

#ifndef CONF_CAN0_TXBC_REG
#define CONF_CAN0_TXBC_REG MCAN_TXBC_TFQS(CONF_CAN0_TXBC_TFQS)
#endif

#ifndef CONF_CAN0_TXEFC_REG
#define CONF_CAN0_TXEFC_REG MCAN_TXEFC_EFWM(CONF_CAN0_TXEFC_EFWM) | MCAN_TXEFC_EFS(CONF_CAN0_TXEFC_EFS)
#endif

#ifndef CONF_CAN0_GFC_REG
#define CONF_CAN0_GFC_REG                                                                                              \
	MCAN_GFC_ANFS(CONF_CAN0_GFC_ANFS) | MCAN_GFC_ANFE(CONF_CAN0_GFC_ANFE) | (CONF_CAN0_GFC_RRFS << MCAN_GFC_RRFS_Pos)  \
	    | (CONF_CAN0_GFC_RRFE << MCAN_GFC_RRFE_Pos)
#endif

#ifndef CONF_CAN0_SIDFC_REG
#define CONF_CAN0_SIDFC_REG MCAN_SIDFC_LSS(CONF_CAN0_SIDFC_LSS)
#endif

#ifndef CONF_CAN0_XIDFC_REG
#define CONF_CAN0_XIDFC_REG MCAN_XIDFC_LSE(CONF_CAN0_XIDFC_LSS)
#endif

#ifndef CONF_CAN0_XIDAM_REG
#define CONF_CAN0_XIDAM_REG MCAN_XIDAM_EIDM(CONF_CAN0_XIDAM_EIDM)
#endif

#ifndef CONF_CAN0_IE_REG
#define CONF_CAN0_IE_REG                                                                                               \
	(CONF_CAN0_IE_EW << MCAN_IR_EW_Pos) | (CONF_CAN0_IE_EA << MCAN_IR_EP_Pos) | (CONF_CAN0_IE_EP << MCAN_IR_EP_Pos)    \
	    | (CONF_CAN0_IE_BO << MCAN_IR_BO_Pos) | (CONF_CAN0_IE_DO << MCAN_IR_RF0L_Pos)
#endif

// <<< end of configuration section >>>

#endif // HPL_MCAN_CONFIG_H
