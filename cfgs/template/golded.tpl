;   ----------------------------------------------------------------------
;               Sample message template file for GoldED
;   ----------------------------------------------------------------------
;
@LoadLanguage GEDLNGUS.CFG
;
;   ----------------------------------------------------------------------
;   Insert these lines if the message is a moved reply.
;   ----------------------------------------------------------------------
@Moved
@Moved*** Answering a msg posted in area @OEcho (@ODesc).
@Moved
;
;   ----------------------------------------------------------------------
;   Insert these lines if the message is changed, and not from you.
;   ----------------------------------------------------------------------
@Changed
@Changed*** Changed by @CName (@CAddr), @CDate @CTime.
@Changed
;
;   ----------------------------------------------------------------------
;   Insert these lines if the message is forwarded.
;   ----------------------------------------------------------------------
@Forward=============================================================================
@Forward* Forwarded by @CName (@CAddr)
@Forward* Area : @OEcho (@ODesc)
@Forward* From : @OName, @OAddr (@ODate @OTime)
@Forward* To   : @DName
@Forward* Subj : @Subject
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
Hello @pseudo!
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
@ReplyAnswering a msg of <@ODate>, from @OName{me}{you} to @DName{me}{you}:
@Reply@Position
;
;   ----------------------------------------------------------------------
;   If we are comment-replying, add these lines.
;   ----------------------------------------------------------------------
@Comment@ODate @OTime, @OName{I}{you} wrote to @DName{me}{you}:
@Comment@Position
;
;   ----------------------------------------------------------------------
;   If we are quote-replying, add these lines.
;   ----------------------------------------------------------------------
@Quoted@ODate @OTime, @OName{I}{you} wrote to @DName{me}{you}:
@Quoted@Position
;
;   ----------------------------------------------------------------------
;   For the quotebuffer, add these lines.
;   ----------------------------------------------------------------------
@Quotebuf
@Quotebuf@ODate @OTime, @OName{I}{you} wrote to @DName{me}{you}:
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
