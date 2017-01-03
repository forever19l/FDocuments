local _Bit_ = {};
print("< Bit_Operation > Load utils.Bit_Operation.lua ...")
--[[---------------------------------
--Version : V1.0
--Author  : Yuekie
--Date    : 20161126
--Comment : First Relese

--Version : V1.1
--Author  : Yuekie
--Date    : 20161126
--Comment : Modify "Xor" function
			Add "GetBit" function take place of "get_bit" 

--Version : V1.2
--Author  : Yuekie
--Date    : 20161127
--Comment : Add "BitsCheck" function for Data check
--]]---------------------------------

-- local function get_bit(data,bit)
--     if(bit == 0) then
--         return math.mod(data,2)
--     else
--         return math.mod(math.floor((data/math.pow(2,bit))),2)
--     end
-- end

local function BitsCheck(Bits,SourceData1,SourceData2)
    -- body
    local Limit = 2^Bits;
    local NewData1 = SourceData1;
    local NewData2 = SourceData2;
    if NewData1 > Limit then
        NewData1 = NewData1%Limit;
    end

    if NewData2 and NewData2 > Limit then
        NewData2 = NewData2%Limit;
    end

    return NewData1,NewData2;
end

function _Bit_.GetBit(Bits,SourceData1,bit)

	if Bits <= bit then
		return nil;
	end
	local Data1 = BitsCheck(Bits,SourceData1);

	local NewData=0;
	local Temp=0;
	for i=Bits-1,bit+1,-1 do
		Temp=2^i;
		if Data1>=Temp then 
			Data1=Data1-Temp; 	
		end
	end

	if Data1 >= 2^bit then
		return 1;
	end
	return 0;
end

function _Bit_.Shift(NBits,SourceData,Flag,SBits)
	local NewData=0;

	local Data = BitsCheck(NBits,SourceData);
	if (Flag == "L") then --Left
		NewData = Data*(2^SBits);
	elseif (Flag == "R") then --Right
		--NewData = Data*(2^Bits);
		local Temp=0;
		for i=NBits-1,SBits,-1 do
			Temp=2^i;
			if Data>=Temp then 
				Data=Data-Temp; 
				NewData=NewData+2^(i-SBits);
			end
		end
	end
	return NewData;
end

function _Bit_.LShift(Data,SBits)
	local NewData=0;
	NewData = Data*(2^SBits);
	return NewData;
end

function _Bit_.RShift(Data,SBits)
	local NewData=0;
	NewData = math.floor(Data/(2^SBits));
	return NewData;
end

function _Bit_.And(Bits,SourceData1,SourceData2)
	local NewData=0;
	local Temp=0;
	local Data1,Data2 = BitsCheck(Bits,SourceData1,SourceData2);
	for i=Bits-1,0,-1 do
		Temp=2^i;
		if Data1>=Temp then 
			Data1=Data1-Temp; 
			if Data2>=Temp then
				Data2=Data2-Temp; 
				NewData=NewData+Temp;
			end
		elseif Data2>=Temp then
			Data2=Data2-Temp;
		end
	end
	return NewData;
end

function _Bit_.Or(Bits,SourceData1,SourceData2)
	local NewData=0;
	local Flag=false;
	local Temp=0;
	local Data1,Data2 = BitsCheck(Bits,SourceData1,SourceData2);
	for i=Bits-1,0,-1 do
		Flag=false;
		Temp=2^i;
		if Data1>=Temp then 
			Data1=Data1-Temp; 
			Flag=true;	
			end
		if Data2>=Temp then
			Data2=Data2-Temp; 
			Flag=true;
		end
		if (Flag==true) then
			NewData=NewData+Temp;
		end
	end
	return NewData;
end


-- function _Bit_.Xor(Bits,SourceData1,SourceData2)
--     local NewData=0;
--     local Data1,Data2 = BitsCheck(Bits,SourceData1,SourceData2);
--     for i=Bits-1,0,-1 do
--         local Bit1 = get_bit(Data1,i);
--         local Bit2 = get_bit(Data2,i);

--         if Bit1 ~= Bit2 then
--             NewData = NewData + 2^i;
--         end
--     end
--     return NewData;
-- end

function _Bit_.Xor(Bits,SourceData1,SourceData2)
    local NewData=0;
    local Flag=false;
    local Flag2 = false;
    local Temp=0;
    local Data1,Data2 = BitsCheck(Bits,SourceData1,SourceData2);
    for i=Bits-1,0,-1 do
        Flag=false;
        Flag2=false;
        Temp=2^i;
        if Data1>=Temp then 
            Data1=Data1-Temp; 
            Flag=true;
        end
        if Data2>=Temp then
            Data2=Data2-Temp; 
            Flag2=true;
        end
        if (Flag~=Flag2) then
            NewData=NewData+Temp;
        end
    end
    return NewData;
end

function _Bit_.Not(Bits,SourceData)
	local Data = BitsCheck(Bits,SourceData);
	local NewData = 2^Bits-1-Data;
	return NewData;
end

print("< Bit_Operation > Load utils.Bit_Operation.lua Success")

return _Bit_;