; ModuleID = 'test/Loop.ll'
source_filename = "test/Loop.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@g = dso_local global i32 0, align 4

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @g_incr(i32 %0) #0 {
  %2 = load i32, i32* @g, align 4
  %3 = add nsw i32 %2, %0
  store i32 %3, i32* @g, align 4
  %4 = load i32, i32* @g, align 4
  ret i32 %4
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @loop(i32 %0, i32 %1, i32 %2) #0 {
  br label %4

4:                                                ; preds = %8, %3
  %.0 = phi i32 [ %0, %3 ], [ %9, %8 ]
  %5 = icmp slt i32 %.0, %1
  br i1 %5, label %6, label %10

6:                                                ; preds = %4
  %7 = call i32 @g_incr(i32 %2)
  br label %8

8:                                                ; preds = %6
  %9 = add nsw i32 %.0, 1
  br label %4, !llvm.loop !4

10:                                               ; preds = %4
  %11 = load i32, i32* @g, align 4
  %12 = add nsw i32 0, %11
  ret i32 %12
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 1}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"Ubuntu clang version 13.0.0-2"}
!4 = distinct !{!4, !5}
!5 = !{!"llvm.loop.mustprogress"}
