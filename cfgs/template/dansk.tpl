;   ----------------------------------------------------------------------
;               Sample message template file for GoldED
;   ----------------------------------------------------------------------
;
@LoadLanguage gedlngdk.cfg
;
;   ----------------------------------------------------------------------
;   Insert these lines if the message is a moved reply.
;   ----------------------------------------------------------------------
@Moved
@Moved*** Svar p† brev postet i omr†de @OEcho (@ODesc).
@Moved
;
;   ----------------------------------------------------------------------
;   Insert these lines if the message is changed, and not from you.
;   ----------------------------------------------------------------------
@Changed
@Changed*** ’ndret af @CName (@CAddr), @CDate @CTime.
@Changed
;
;   ----------------------------------------------------------------------
;   Insert these lines if the message is forwarded.
;   ----------------------------------------------------------------------
@Forward=============================================================================
@Forward* Forwarded af @CName (@CAddr)
@Forward* Area : @OEcho (@ODesc)
@Forward* Fra  : @OName, @OAddr (@ODate @OTime)
@Forward* Til  : @DName
@Forward* Emne : @Subject
@Forward=============================================================================
;
;   ----------------------------------------------------------------------
;   If we are forwarding or changing, the original message is inserted
;   here, with the @Message token.
;   ----------------------------------------------------------------------
@Message
;
@Forward=============================================================================
@Forward
;
;   ----------------------------------------------------------------------
;   Normal template begins.
;   ----------------------------------------------------------------------
Hej @TFName!
;
;   ----------------------------------------------------------------------
;   Insert extra empty line in new messages (non-replies).
;   ----------------------------------------------------------------------
@New
;
;   ----------------------------------------------------------------------
;   Position cursor for the editor.
;   ----------------------------------------------------------------------
@Position
;
;   ----------------------------------------------------------------------
;   If we are replying without quoting, add these lines.
;   ----------------------------------------------------------------------
@ReplySvar p† brev af <@ODate>, fra @OName til @DName:
@Reply@Position
;
;   ----------------------------------------------------------------------
;   If we are comment-replying, add these lines.
;   ----------------------------------------------------------------------
@Comment@ODate kl.@OTime skrev @OName til @DName:
@Comment@Position
;
;   ----------------------------------------------------------------------
;   If we are quote-replying, add these lines.
;   ----------------------------------------------------------------------
@Quoted@ODate kl.@OTime skrev @OName til @DName:
@Quoted@Position
;
;   ----------------------------------------------------------------------
;   For the quotebuffer, add these lines.
;   ----------------------------------------------------------------------
@Quotebuf
@Quotebuf@ODate kl.@OTime skrev @OName til @DName:
@Quotebuf
;
;   ----------------------------------------------------------------------
;   Insert quote of the original message here, if our message is a
;   comment-reply or quote-reply.
;   ----------------------------------------------------------------------
@Quote

;   ----------------------------------------------------------------------
;   Sign the message with your first name.
;   ----------------------------------------------------------------------
@CFName

;   ----------------------------------------------------------------------
;   End of template. Confused? Me too! :-)
;   ----------------------------------------------------------------------
