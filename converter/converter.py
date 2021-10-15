from errors import ParenthesisNotMatchError
import json


class Converter(object):
    """OIers in Restart events converter"""

    def __init__(self) -> None:
        super().__init__()
        self.EVENT_TYPE_NORMAL = 'normal'
        self.EVENT_TYPE_NORMAL_CODE = ''
        self.EVENT_TYPE_REPEAT = 'repeat'
        self.EVENT_TYPE_REPEAT_CODE = '+'
        self.EVENT_TYPE_GLOBAL = 'global'
        self.EVENT_TYPE_GLOBAL_CODE = '*'
        self.EVENT_TYPE_JUMP = 'jump'

    def __find_matching_paretheses(self, s: str, parens_left='(', parens_right=')') -> dict:
        """Find all matching parentheses and store their indexes

        Args:
            s (str): string to index parentheses
            parens_left (str, optional): the left parethese to match. Defaults to '('.
            parens_right (str, optional): the right parethese to match. Defaults to ')'.

        Raises:
            ParenthesisNotMatchError: raises when there's a mismatch of parentheses

        Returns:
            dict: indexes of matched parentheses. formats like `{left_index: right_index}`.
        """
        stack = []
        result = {}
        for idx, ch in enumerate(s):
            if ch == parens_left:
                stack.append(idx)
            elif ch == parens_right:
                if not len(stack):
                    raise ParenthesisNotMatchError(idx)
                result[stack.pop()] = idx
        if len(stack):
            raise ParenthesisNotMatchError(stack.pop())
        return result

    def convert(self, filename: str, output_filename: str = 'events.json') -> list:
        """Converts plaintext events to JSON

        Args:
            filename (str): the plaintext filename to convert
            output_filename (str, optional): the JSON filename (path) to the store to. Defaults to 'events.json'.

        Returns:
            list: the converted events list
        """
        result = []
        content = []
        # read and parse the events
        for ln in open(filename, 'r', encoding='utf-8').readlines():
            if ln.strip('\n'):
                content.append(ln.strip('\n'))
            else:
                continue
        # parse the events one by one
        for event in content:
            if event.startswith('#'):
                continue  # ignore comments
            parens = self.__find_matching_paretheses(
                event)  # parenthesis match
            parens_brackets = self.__find_matching_paretheses(
                event, '[', ']')  # parenthesis match for brackets `[]`
            evt_id = event.split(' ', 1)[0].strip()  # event ID
            evt_type = self.EVENT_TYPE_NORMAL  # event type, default to type `normal`
            # check for special events
            if evt_id[0] == self.EVENT_TYPE_REPEAT_CODE:
                evt_type = self.EVENT_TYPE_REPEAT  # event may repeat
            elif evt_id[0] == self.EVENT_TYPE_GLOBAL_CODE:
                evt_type = self.EVENT_TYPE_GLOBAL  # event may occur on any stages
            if not evt_id[0].isdigit():
                evt_id = evt_id[1:]  # remove leading type character if any
            evt_id = int(evt_id)  # convert event id to integer
            evt_condition_end = parens[event.find('(')]
            # event condition, default to `None`
            evt_condition = event[event.find(
                '(') + 1:evt_condition_end].strip()
            evt_result_start = event.find('(', evt_condition_end + 1)
            evt_result_end = parens[evt_result_start]
            # event result, which will happen after this event
            evt_result_str = event[evt_result_start + 1:evt_result_end].strip()
            evt_triggers_start = event.find('[', evt_result_end + 1)
            evt_triggers_end = parens_brackets[evt_triggers_start]
            # events that this event will CERTAINLY trigger
            evt_triggers_str = event[evt_triggers_start +
                                     1:evt_triggers_end].strip()
            evt_possibility_up_start = event.find('[', evt_triggers_end + 1)
            evt_possibility_up_end = parens_brackets[evt_possibility_up_start]
            # events that will be more LIKELY to happen after this event
            evt_possibility_up_str = event[evt_possibility_up_start +
                                           1:evt_possibility_up_end].strip()
            evt_msg = event[evt_possibility_up_end +
                            1:].strip()  # event message
            if evt_msg.startswith('Jump to'):  # special case for `jump to event`
                evt_type = self.EVENT_TYPE_JUMP
                # the message is the event ID to jump to
                evt_msg = int(evt_msg.strip('Jump to '))
            # serialize the events
            evt_result = []
            for res in evt_result_str.split(','):
                if res.strip().strip('#'):
                    evt_result.append(res.strip())
            evt_triggers = []
            for trigger in evt_triggers_str.split(','):
                if trigger.strip():
                    evt_triggers.append(int(trigger.strip()))
            evt_possibility_up = []
            for evt in evt_possibility_up_str.split(','):
                if evt.strip():
                    evt_possibility_up.append(int(evt.strip()))
            ##########   Debug   ##########
            # print('Condition:', evt_condition)
            # print('Result:', evt_result)
            # print('Triggers:', evt_triggers)
            # print ('Possibility up:', evt_possibility_up)
            # print('Message:', evt_msg)
            # print('-------------')
            ########## End Debug ##########
            # append current event to the list
            result.append({
                'id': evt_id,
                'type': evt_type,
                'condition': evt_condition,
                'result': evt_result,
                'triggers': evt_triggers,
                'possibility_up': evt_possibility_up,
                'msg': evt_msg
            })
        # write results to file
        with open(output_filename, 'w', encoding='utf-8') as f:
            # dump the Python list to JSON and store it (with a 2-spaces indent)
            f.write(json.dumps(result, indent=2))
        return result


# Demo
if __name__ == '__main__':
    converter = Converter()
    converter.convert('./events.txt')
